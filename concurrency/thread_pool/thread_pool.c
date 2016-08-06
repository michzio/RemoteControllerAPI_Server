//
// Created by Michal Ziobro on 28/07/2016.
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <signal.h>
#include "thread_pool.h"
#include "../../common/array_helper.h"
#include "../../config.h"
#include "../../collections/doubly_linked_list.h"

const size_t default_pool_size = 8;        // default num of workers if not specified by user
const size_t default_max_pool_size = 16;    // max num of workers if not specified by user
const int default_worker_ms_timeout = 1000; // 1000 mili seconds of timeout

struct thread_pool {
    task_queue_t *task_queue;   // shared queue storing tasks to be run by workers
    pthread_t *workers;         // worker threads running tasks from queue
    int workers_count;          // current number of workers
    int workers_limit_min;      // min number of workers in the pool
    int workers_limit_max;      // max number of workers in the pool
    int worker_ms_timeout;      // worker max time in ms to wait for new task until it will be killed

    doubly_linked_list_t *deleted_workers; // deleted worker threads that should be joined to release resources
    bool shutting_down;          // flag indicating if thread pool is shutting down currently or not
};

// synchronization of access to thread pool (modification of workers and its counter)
static pthread_mutex_t mutex;
// synchronization of access to thread pool's is_paused flag used while pausing and resuming worker threads
static pthread_mutex_t pause_threads_mutex;
static pthread_cond_t pause_threads_cond;

static bool is_paused = 0;       // flag indicating if thread pool workers are paused or not

static void cleanup_unlock_mutex(void *p)
{
    pthread_mutex_unlock(p);
}

static void remove_worker(thread_pool_t *thread_pool, pthread_t worker_thread) {

    unsigned int worker_idx;

    // 1. find index of worker in workers array
    worker_idx = array_find( (const void **) thread_pool->workers, thread_pool->workers_count, worker_thread, (int (*)(const void *, const void *)) pthread_equal);
    // 2. remove element based on it's index, and move remaining elements to get rid of the hole
    array_remove( (void **) thread_pool->workers, thread_pool->workers_count, worker_idx);
    // 3. modify workers counter
    (thread_pool->workers_count)--;
    // 4. add removed worker to trash
    push_front(thread_pool->deleted_workers, (void *) worker_thread, sizeof(worker_thread));

    if(DEBUG) {
        printf("worker thread: %p has been removed from thread pool.\n", worker_thread);
        array_print( (const void **) thread_pool->workers, thread_pool->workers_count, pointer_printer);
    }
}

static void join_deleted_workers(thread_pool_t *thread_pool) {

    pthread_t worker_thread;
    doubly_linked_node_t *node;

    pthread_mutex_lock(&mutex);

    while( (node = back(thread_pool->deleted_workers)) != NULL )
    {
        worker_thread = (pthread_t) unwrap_data(node, NULL);
        pop_back(thread_pool->deleted_workers);
        pthread_join(worker_thread, NULL); // joining deleted worker threads
        free(worker_thread);
    }

    pthread_mutex_unlock(&mutex);
}

static void pause_thread_signal_handler(int arg) {

    pthread_mutex_lock(&pause_threads_mutex);

    is_paused = 1;

    if(DEBUG)
        fprintf(stderr, "worker thread: %p has been paused...\n", pthread_self());

    while (is_paused != 0)
        pthread_cond_wait(&pause_threads_cond, &pause_threads_mutex);

    if(DEBUG)
        fprintf(stderr, "worker thread: %p has been resumed...\n", pthread_self());

    pthread_mutex_unlock(&pause_threads_mutex);
}

static void register_worker_signal_handler() {

    struct sigaction sigact;

    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = pause_thread_signal_handler;

    if (sigaction(SIGUSR1, &sigact, NULL) == -1) {
        fprintf(stderr, "worker(): cannot handle SIGUSR1.\n");
    }
}

// worker
static void *worker(thread_pool_t *thread_pool) {

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    register_worker_signal_handler();

    // infinite loop consecutively executing available tasks
    while(1) {
        // 1. look for new task to be done
        task_t * task;
        if(thread_pool->worker_ms_timeout > 0)
            task = dequeue_task_timed(thread_pool->task_queue, thread_pool->worker_ms_timeout);
        else
            task = dequeue_task(thread_pool->task_queue);
        // 2. check task availability, if waiting for task too long, kill worker
        if(task == NULL) {
            pthread_mutex_lock(&mutex);
            pthread_cleanup_push(cleanup_unlock_mutex, (void *) &mutex);
            if(thread_pool->workers_count > thread_pool->workers_limit_min) {
                // there is too many starving workers waiting for tasks, kill current worker
                remove_worker(thread_pool, pthread_self());
                // pthread exit will pop cleanup handler and execute it (i.e. unlock mutex)
                pthread_exit(PTHREAD_CANCELED);
            } else {
                // min number of workers in thread pool, current worker cannot be killed
                pthread_cleanup_pop(0);
                pthread_mutex_unlock(&mutex);
                continue;
            }
        }
        // 3. do task
        task_run(task);
        // 4. free task
        task_free(task);
    }
}

// thread pool operations

result_t thread_pool_init_default(thread_pool_t **thread_pool) {
    return thread_pool_init(thread_pool, default_pool_size, default_max_pool_size, default_worker_ms_timeout);
}

result_t thread_pool_init(thread_pool_t **thread_pool, const size_t size, const size_t max_size, const int worker_ms_timeout) {

    if (size > max_size) {
        fprintf(stderr, "thread pool initialization failed, size is larger then max size!\n");
        return FAILURE;
    }

    // allocation of memory for thread pool and internal task queue
    *thread_pool = malloc(sizeof(thread_pool_t));
    task_queue_init(&((*thread_pool)->task_queue));

    // allocation of memory for workers array
    (*thread_pool)->workers = (pthread_t *) malloc(sizeof(pthread_t) * max_size);
    (*thread_pool)->workers_count = 0;
    (*thread_pool)->workers_limit_min = size;
    (*thread_pool)->workers_limit_max = max_size;
    (*thread_pool)->worker_ms_timeout = worker_ms_timeout;
    (*thread_pool)->shutting_down = 0;
    is_paused = 0;

    // allocation of memory for deleted workers linked list
    list_init(&((*thread_pool)->deleted_workers), NULL);

    // init synchronization objects: mutex, pause threads mutex, pause threads cond var
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&pause_threads_mutex, NULL);
    pthread_cond_init(&pause_threads_cond, NULL);

    // create worker threads
    pthread_attr_t pthread_attr;
    pthread_attr_init(&pthread_attr);
    // pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED); // worker threads will be created detached --> DEPRECATED

    // protect initial creation of worker threads, in order to not corrupt thread pool
    pthread_mutex_lock(&mutex);

    for (int i = 0; i < size; i++) {
        if (pthread_create(&((*thread_pool)->workers[i]), &pthread_attr, (void *(*)(void *)) worker,
                           (void *) *thread_pool) != 0) {
            fprintf(stderr, "pthread_create: failed to create worker thread.\n");
            return FAILURE;
        }
        (*thread_pool)->workers_count++;
    }

    if ((*thread_pool)->workers_count != size) {
        fprintf(stderr, "thread pool initialization failed, not created all worker threads!\n");
    }

    if (DEBUG) {
        printf("created worker threads:\n");
        array_print((const void **) (*thread_pool)->workers, (*thread_pool)->workers_count, pointer_printer);
    }

    pthread_mutex_unlock(&mutex);

    return SUCCESS;
}

void thread_pool_execute(thread_pool_t *thread_pool, task_t *task) {

    if(thread_pool == NULL || task == NULL) {
        fprintf(stderr, "thread_pool_execute: thread pool or task is empty!\n");
        return;
    }
    if(thread_pool->shutting_down) {
        fprintf(stderr, "thread_pool_execute: thread pool is currently shutting down!\n");
        return;
    }

    enqueue_task( thread_pool->task_queue, task );
}

void thread_pool_run(thread_pool_t *thread_pool, runner_t runner, runner_attr_t runner_attr, runner_res_handler_t runner_res_handler) {

    if(thread_pool == NULL || runner == NULL) {
        fprintf(stderr, "thread_pool_run: thread pool or runner is empty!\n");
        return;
    }
    if(thread_pool->shutting_down) {
        fprintf(stderr, "thread_pool_run: thread pool is currently shutting down!\n");
        return;
    }

    task_t *task;
    task_init(&task);

    // populate task with runner, its attributes and result handler
    task_fill(task, runner, runner_attr, runner_res_handler);

    enqueue_task( thread_pool->task_queue, task );
}

result_t thread_pool_adjust_size(thread_pool_t *thread_pool) {

    // release resources occupied by removed worker threads by joining them
    join_deleted_workers(thread_pool);

    // if needed create more worker threads to execute tasks
    pthread_mutex_lock(&mutex);

    int task_count = task_queue_count(thread_pool->task_queue);
    if(task_count > thread_pool->workers_limit_max) task_count = thread_pool->workers_limit_max;

    int demand = task_count - thread_pool->workers_count;
    int workers_count = thread_pool->workers_count;

    // create additional worker threads
    pthread_attr_t pthread_attr;
    pthread_attr_init(&pthread_attr);

    for(int i = workers_count; i < (workers_count + demand); i++) {
        if (pthread_create(&(thread_pool->workers[i]), &pthread_attr, (void *(*)(void *)) worker, (void *) thread_pool) != 0) {
            fprintf(stderr, "pthread_create: failed to create additional worker thread.\n");
            return FAILURE;
        }
        (thread_pool->workers_count)++;
    }

    if (DEBUG) {
        printf("worker threads after adjusting:\n");
        array_print((const void **) thread_pool->workers, thread_pool->workers_count, pointer_printer);
    }

    pthread_mutex_unlock(&mutex);

    return SUCCESS;
}

/* function pause all threads in thread pool */
void thread_pool_pause(thread_pool_t* thread_pool) {

    pthread_mutex_lock(&mutex); // to prevent modification of workers array, and workers counter

    for (int i=0; i < thread_pool->workers_count; i++) {
        pthread_kill(thread_pool->workers[i], SIGUSR1);
    }

    pthread_mutex_unlock(&mutex);
}

/* function resume all threads in thread pool */
void thread_pool_resume(thread_pool_t *thread_pool) {

    // take mutex to change thread pool's is_paused flag
    pthread_mutex_lock(&pause_threads_mutex);

    is_paused = 0;
    pthread_cond_signal(&pause_threads_cond);

    // release mutex
    pthread_mutex_unlock(&pause_threads_mutex);
}

/**
 * function shutdowns gracefully thread pool
 * waiting until all pending tasks will be
 * executed and starving worker threads will
 * be removed by itself. It needs to set
 * workers_limit_min to 0 (enabling to remove
 * all existing worker threads), worker_ms_timeout
 * to low value ex. 10ms (fast starvation of worker threads
 * on pthread_cond_timedwait() ). Than it needs to join
 * all self terminating worker threads. Then it frees
 * resources associated with thread pool.
 */
void thread_pool_shutdown(thread_pool_t *thread_pool) {

    int workers_count;
    pthread_t *workers;

    if(is_paused) thread_pool_resume(thread_pool);

    pthread_mutex_lock(&mutex); // lock mutex to stop removing workers

    join_deleted_workers(thread_pool);

    // get remaining worker threads to remove
    workers_count = thread_pool->workers_count;
    workers = malloc(sizeof(pthread_t) * workers_count);
    for(int i=0; i<workers_count; i++) {
        workers[i] = thread_pool->workers[i];
    }

    thread_pool->shutting_down = 1; // shutting down mode - disable addition of new tasks
    thread_pool->workers_limit_min = 0; // enable deletion of all worker threads - NO min limit
    thread_pool->worker_ms_timeout = 10; // short time of waiting for tasks

    pthread_mutex_unlock(&mutex); // unlock mutex to start removing all starving workers

    // join remaining worker threads
    for(int i=0; i<workers_count; i++)
        pthread_join(workers[i], NULL);

    // deallocate internal array of workers
    for(int i=0; i<workers_count; i++)
        free(thread_pool->workers[i]);
    free(thread_pool->workers);
    // deallocate internal list of deleted workers
    list_free(thread_pool->deleted_workers);
    //deallocate internal task queue
    task_queue_free(thread_pool->task_queue);
    // deallocate thread pool
    free(thread_pool);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&pause_threads_mutex);
    pthread_cond_destroy(&pause_threads_cond);
}

/**
 * function forcefully free thread pool and associated resources
 * like worker threads and task queue. It doesn't wait to finish
 * pending tasks in the queue, rather cancel worker threads
 * in the nearest possible cancellation point.
 */
void thread_pool_force_free(thread_pool_t *thread_pool) {

    int workers_count;

    if(is_paused) thread_pool_resume(thread_pool);

    // disable addition of new tasks
    thread_pool->shutting_down = 1;

    // reset workers count to prevent workers self killing while cancelling them
    pthread_mutex_lock(&mutex);
    workers_count = thread_pool->workers_count;
    thread_pool->workers_count = 0;
    pthread_mutex_unlock(&mutex);

    for(int i=0; i<workers_count; i++)
        pthread_cancel(thread_pool->workers[i]);

    // join workers to release their resources
    for(int i=0; i<workers_count; i++)
        pthread_join(thread_pool->workers[i], NULL);

    // release resources occupied by removed worker threads by joining them
    join_deleted_workers(thread_pool);

    // deallocate internal array of workers
    free(thread_pool->workers);
    // deallocate internal list of deleted workers
    list_free(thread_pool->deleted_workers);
    //deallocate internal task queue
    task_queue_free(thread_pool->task_queue);
    // deallocate thread pool
    free(thread_pool);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&pause_threads_mutex);
    pthread_cond_destroy(&pause_threads_cond);
}