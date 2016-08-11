//
// Created by Michal Ziobro on 08/08/2016.
//

#include <ntsid.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include "../common/types.h"
#include "threads_manager.h"
#include "../common/time.h"
#include "thread.h"
#include "../common/array_helper.h"
#include "../config.h"

struct threads_manager {
    pthread_t *threads;
    size_t count;
    size_t max_limit;

    // synchronization objects to manage number of threads running
    pthread_mutex_t mutex;
    pthread_cond_t conditional_variable;
};

static result_t remove_thread(threads_manager_t *threads_manager, pthread_t thread) {

    unsigned int thread_idx;

    // 1. find index of thread in threads array
    if( (thread_idx = array_find( (const void **) threads_manager->threads, threads_manager->count, thread, pthread_cmp_func ) ) == FAILURE) {
        fprintf(stderr, "array_find: failed to find thread: %p in managed threads!\n", thread);
        return FAILURE;
    }
    // 2. remove element based on it's index, and move remaining elements to get rid of the hole
    if( array_remove( (void **) threads_manager->threads, threads_manager->count, thread_idx) == FAILURE) {
        fprintf(stderr, "array_remove: failed to remove thread %p from managed threads by its index: %d!\n", thread, thread_idx);
        return FAILURE;
    }
    // 3. modify threads counter
    (threads_manager->count)--;

    if(DEBUG) {
        printf("thread: %p has been removed from managed threads.\n", thread);
        array_print( (const void **) threads_manager->threads, threads_manager->count, pointer_printer);
    }

    return SUCCESS;
}

static void threads_manager_callback(callback_attr_t callback_attr, runner_res_t arg) {

    threads_manager_t *threads_manager = (threads_manager_t *) callback_attr;

    // 1. take mutex
    pthread_mutex_lock(&threads_manager->mutex);

    // 2. remove thread from threads manager
    if(remove_thread(threads_manager, pthread_self()) == FAILURE) {
        fprintf(stderr, "threads_manager_remove_thread: failed!\n");
        goto UNLOCK_AND_RETURN;
    }

    // 3. signal conditional variable that thread has been removed
    pthread_cond_signal(&threads_manager->conditional_variable);

    // 4. release mutex
    UNLOCK_AND_RETURN:
    pthread_mutex_unlock(&threads_manager->mutex);
}

// threads manager operations
result_t threads_manager_init(threads_manager_t **threads_manager, size_t max_limit) {

    *threads_manager = malloc(sizeof(threads_manager_t));
    (*threads_manager)->threads = malloc(sizeof(pthread_t) * max_limit);
    (*threads_manager)->count = 0;
    (*threads_manager)->max_limit = max_limit;

    pthread_mutex_init(&(*threads_manager)->mutex, NULL);
    pthread_cond_init(&(*threads_manager)->conditional_variable, NULL);

    return SUCCESS;
}

void threads_manager_set_max_limit(threads_manager_t *threads_manager, size_t max_limit) {

    pthread_mutex_lock(&threads_manager->mutex);
    threads_manager->max_limit = max_limit;
    pthread_mutex_unlock(&threads_manager->mutex);
}

size_t threads_manager_max_limit(threads_manager_t *threads_manager) {

    size_t max_limit;

    pthread_mutex_lock(&threads_manager->mutex);
    max_limit = threads_manager->max_limit;
    pthread_mutex_unlock(&threads_manager->mutex);

    return max_limit;
}

size_t threads_manager_count(threads_manager_t *threads_manager) {

    size_t count;

    pthread_mutex_lock(&threads_manager->mutex);
    count = threads_manager->count;
    pthread_mutex_unlock(&threads_manager->mutex);

    return count;
}

pthread_t *threads_manager_threads(threads_manager_t *threads_manager) {

    pthread_t *threads;

    pthread_mutex_lock(&threads_manager->mutex);
    threads = threads_manager->threads;
    pthread_mutex_unlock(&threads_manager->mutex);

    return threads;
}

/**
 * next six functions check whether new thread can be created and max number of threads hasn't been exceeded.
 * they use threads manager object to make sure upper limit of number of threads hasn't been reached.
 * if number of threads has been exceeded than they wait until some current thread terminates and signals this
 */
result_t wait_for_thread(threads_manager_t *threads_manager, runner_t runner, runner_attr_t runner_attr) {

    pthread_t thread;
    result_t result = SUCCESS;
    managed_thread_runner_attr_t *managed_thread_runner_attr;

    // 0. wrap runner and runner attr into managed thread runner attr
    managed_thread_runner_attr_init(&managed_thread_runner_attr);
    managed_thread_runner_attr_fill(managed_thread_runner_attr, runner, runner_attr, threads_manager_callback, (callback_attr_t) threads_manager);

    // 1. take mutex
    pthread_mutex_lock(&threads_manager->mutex);

    // 2. wait until there is space for new thread
    while(threads_manager->count >= threads_manager->max_limit) {
        printf("Max limit of managed threads has been reached. Waiting for space to create new managed thread...\n");
        pthread_cond_wait(&threads_manager->conditional_variable, &threads_manager->mutex);
    }

    // 3. create new thread
    if( (thread = thread_create((runner_t) managed_thread_runner, (runner_attr_t) managed_thread_runner_attr)) == NULL) {
        fprintf(stderr, "thread_create: failed!\n");
        result = FAILURE;
        goto UNLOCK_AND_RETURN;
    }
    pthread_detach(thread); // don't care about joining (releasing) created thread
    threads_manager->threads[threads_manager->count] = thread;
    threads_manager->count++;

    // 4. release mutex
    UNLOCK_AND_RETURN:
    pthread_mutex_unlock(&threads_manager->mutex);

    return result;
}
result_t wait_for_connection_thread(threads_manager_t *threads_manager, connection_handler_t conn_handler, sock_fd_t conn_sock_fd) {

    conn_thread_runner_attr_t *conn_thread_runner_attr;
    conn_thread_runner_attr_init(&conn_thread_runner_attr);
    conn_thread_runner_attr_fill(conn_thread_runner_attr, conn_handler, conn_sock_fd, NULL, NULL);

    return wait_for_thread(threads_manager, (runner_t) connection_thread_runner, (runner_attr_t) conn_thread_runner_attr);
}
result_t wait_for_datagram_thread(threads_manager_t *threads_manager, datagram_handler_t datagram_handler, sock_fd_t sock_fd, struct sockaddr *peer_addr, char *datagram) {

    datagram_thread_runner_attr_t *datagram_thread_runner_attr;
    datagram_thread_runner_attr_init(&datagram_thread_runner_attr);
    datagram_thread_runner_attr_fill(datagram_thread_runner_attr, datagram_handler, sock_fd, peer_addr, datagram, NULL, NULL);

    return wait_for_thread(threads_manager, (runner_t) datagram_thread_runner, (runner_attr_t) datagram_thread_runner_attr);
}

result_t timed_wait_for_thread(threads_manager_t *threads_manager, const int ms_timeout, runner_t runner, runner_attr_t runner_attr) {

    pthread_t thread;
    result_t result = SUCCESS;
    managed_thread_runner_attr_t *managed_thread_runner_attr;
    struct timespec timespec;
    set_timespec_from_timeout(&timespec, ms_timeout);

    // 0. wrap runner and runner attr into managed thread runner attr
    managed_thread_runner_attr_init(&managed_thread_runner_attr);
    managed_thread_runner_attr_fill(managed_thread_runner_attr, runner, runner_attr, threads_manager_callback, (callback_attr_t) threads_manager);

    // 1. take mutex
    pthread_mutex_lock(&threads_manager->mutex);

    // 2. timed wait until there is space for new thread
    while(threads_manager->count >= threads_manager->max_limit)
        if(pthread_cond_timedwait(&threads_manager->conditional_variable, &threads_manager->mutex, &timespec) == ETIMEDOUT) {
            // 3. if waiting timed out return failure
            fprintf(stderr, "Threads manager timed out before creating new thread.\n");
            result = FAILURE;
            goto UNLOCK_AND_RETURN;
        }

    // 4. create new thread
    if( (thread = thread_create((runner_t) managed_thread_runner, (runner_attr_t) managed_thread_runner_attr)) == NULL) {
        fprintf(stderr, "thread_create: failed!\n");
        result = FAILURE;
        goto UNLOCK_AND_RETURN;
    }
    pthread_detach(thread); // don't care about joining (releasing) created thread
    threads_manager->threads[threads_manager->count] = thread;
    threads_manager->count++;

    // 5. release mutex
    UNLOCK_AND_RETURN:
    pthread_mutex_unlock(&threads_manager->mutex);

    return result;
}

result_t timed_wait_for_connection_thread(threads_manager_t *threads_manager, const int ms_timeout, connection_handler_t conn_handler, sock_fd_t conn_sock_fd) {

    conn_thread_runner_attr_t *conn_thread_runner_attr;
    conn_thread_runner_attr_init(&conn_thread_runner_attr);
    conn_thread_runner_attr_fill(conn_thread_runner_attr, conn_handler, conn_sock_fd, NULL, NULL);

    return timed_wait_for_thread(threads_manager, ms_timeout, (runner_t) connection_thread_runner, (runner_attr_t) conn_thread_runner_attr);
}

result_t timed_wait_for_datagram_thread(threads_manager_t *threads_manager, const int ms_timeout, datagram_handler_t datagram_handler, sock_fd_t sock_fd, struct sockaddr *peer_addr, char *datagram) {

    datagram_thread_runner_attr_t *datagram_thread_runner_attr;
    datagram_thread_runner_attr_init(&datagram_thread_runner_attr);
    datagram_thread_runner_attr_fill(datagram_thread_runner_attr, datagram_handler, sock_fd, peer_addr, datagram, NULL, NULL);

    return timed_wait_for_thread(threads_manager, ms_timeout, (runner_t) datagram_thread_runner, (runner_attr_t) datagram_thread_runner_attr);
}

void threads_manager_free(threads_manager_t *threads_manager) {

    free(threads_manager->threads);
    pthread_mutex_destroy(&threads_manager->mutex);
    pthread_cond_destroy(&threads_manager->conditional_variable);
    free(threads_manager);
}