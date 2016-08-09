//
// Created by Michal Ziobro on 08/08/2016.
//

#include <ntsid.h>
#include <stdlib.h>
#include <pthread.h>
#include "../common/types.h"
#include "threads_manager.h"
#include "../common/time.h"

struct threads_manager {
    pthread_t *threads;
    size_t count;
    size_t max_limit;
};

// synchronization objects to manage number of threads running
static pthread_mutex_t mutex;
static pthread_cond_t conditional_variable;

// threads manager operations
result_t threads_manager_init(threads_manager_t **threads_manager, size_t max_limit) {

    *threads_manager = malloc(sizeof(threads_manager_t));
    (*threads_manager)->threads = malloc(sizeof(pthread_t) * max_limit);
    (*threads_manager)->count = 0;
    (*threads_manager)->max_limit = max_limit;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&conditional_variable, NULL);
}

/**
 * next four functions check whether new thread can be created and max number of threads hasn't been exceeded.
 * they use threads manager object to make sure upper limit of number of threads hasn't been reached.
 * if number of threads has been exceeded than they wait until some current thread terminates and signals this
 */
result_t wait_for_connection_thread(threads_manager_t *threads_manager, connection_handler_t conn_handler, sock_fd_t conn_sock_fd) {

    pthread_mutex_lock(&mutex);

    while(threads_manager->count == threads_manager->max_limit)
        pthread_cond_wait(&conditional_variable, &mutex);

    // create new connection thread

    threads_manager->count++;

    pthread_mutex_unlock(&mutex);

}
result_t wait_for_datagram_thread(threads_manager_t *threads_manager, datagram_handler_t datagram_handler, sock_fd_t sock_fd, const struct sockaddr *peer_addr, const char *datagram) {

    pthread_mutex_lock(&mutex);

    while(threads_manager->count == threads_manager->max_limit)
        pthread_cond_wait(&conditional_variable, &mutex);

    // create new datagram thread

    threads_manager->count++;

    pthread_mutex_unlock(&mutex);

}
result_t timed_wait_for_connection_thread(threads_manager_t *threads_manager, const int ms_timeout, connection_handler_t conn_handler, sock_fd_t conn_sock_fd) {

    struct timespec timespec;
    set_timespec_from_timeout(&timespec, ms_timeout);

    pthread_mutex_lock(&mutex);

    while(threads_manager->count == threads_manager->max_limit)
        pthread_cond_timedwait(&conditional_variable, &mutex, &timespec);

    // create new connection thread

    threads_manager->count++;

    pthread_mutex_unlock(&mutex);

}
result_t timed_wait_for_datagram_thread(threads_manager_t *threads_manager, const int ms_timeout, datagram_handler_t datagram_handler, sock_fd_t sock_fd, const struct sockaddr *peer_addr, const char *datagram) {

    struct timespec timespec;
    set_timespec_from_timeout(&timespec, ms_timeout);

    pthread_mutex_lock(&mutex);

    while(threads_manager->count == threads_manager->max_limit)
        pthread_cond_timedwait(&conditional_variable, &mutex, &timespec);

    // create new datagram thread

    threads_manager->count++;

    pthread_mutex_unlock(&mutex);

}

void threads_manager_free(threads_manager_t *threads_manager) {

    free(threads_manager->threads);
    free(threads_manager);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&conditional_variable);
}