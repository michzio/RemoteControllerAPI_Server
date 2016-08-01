//
// Created by Michal Ziobro on 28/07/2016.
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "thread_pool.h"
#include "../../common/array_helper.h"
#include "../../config.h"
#include "../../collections/doubly_linked_list.h"

const size_t default_pool_size = 10;        // default num of workers if not specified by user
const size_t default_max_pool_size = 20;    // max num of workers if not specified by user
const int default_worker_ms_timeout = 1000; // 1000 mili seconds of timeout

struct thread_pool {
    task_queue_t *task_queue;   // shared queue storing tasks to be run by workers
    pthread_t *workers;         // worker threads running tasks from queue
    int workers_count;          // current number of workers
    int workers_limit_min;      // min number of workers in the pool
    int workers_limit_max;      // max number of workers in the pool
    int worker_ms_timeout;      // worker max time in ms to wait for new task until it will be killed

    doubly_linked_list_t *deleted_workers; // deleted worker threads that should be joined to release resources
};

// synchronization of access to thread pool (modification of workers and its counter)
static pthread_mutex_t mutex;

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
    }

    pthread_mutex_unlock(&mutex);
}

// worker
static void *worker(thread_pool_t *thread_pool) {

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

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
    (*thread_pool)->workers_limit_min = size;
    (*thread_pool)->workers_limit_max = max_size;
    (*thread_pool)->worker_ms_timeout = worker_ms_timeout;

    // init synchronization objects: mutex
    pthread_mutex_init(&mutex, NULL);

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

    enqueue_task( thread_pool->task_queue, task );
}

void thread_pool_run(thread_pool_t *thread_pool, runner_t runner, runner_attr_t runner_attr, runner_res_handler_t runner_res_handler) {

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

void thread_pool_free(thread_pool_t *thread_pool) {

    // reset workers count to prevent workers self killing while cancelling them
    pthread_mutex_lock(&mutex);
    int workers_count = thread_pool->workers_count;
    thread_pool->workers_count = 0;
    pthread_mutex_unlock(&mutex);

    // join workers to release their resources
    for(int i=0; i < workers_count; i++) {
        pthread_cancel(thread_pool->workers[i]);
        pthread_join(thread_pool->workers[i], NULL);
    }
    // deallocate internal array of workers
    free(thread_pool->workers);

    // release resources occupied by removed worker threads by joining them
    join_deleted_workers(thread_pool);
    // deallocate internal list of deleted workers
    free_doubly_linked_list(thread_pool->deleted_workers);

    //deallocate internal task queue
    task_queue_free(thread_pool->task_queue);

    // reset remaining attributes of thread pool
    thread_pool->workers_limit_min = 0;
    thread_pool->workers_limit_max = 0;
    thread_pool->worker_ms_timeout = 0;

    free(thread_pool);
}