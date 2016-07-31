//
// Created by Michal Ziobro on 28/07/2016.
//

#include <ntsid.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "thread_pool.h"
#include "task_queue.h"
#include "../../common/array_helper.h"

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
};

// synchronization of access to thread pool (modification of workers and its counter)
static pthread_mutex_t mutex;

static void remove_worker(thread_pool_t *thread_pool, pthread_t worker) {

    int worker_idx;

    // 1. find index of worker in workers array
    worker_idx = array_find(thread_pool->workers, thread_pool->workers_count, worker, pthread_equal);
    // 2. remove element, and move remaining elements to get rid of the hole

    // 3. modify workers counter

}

// worker
static void *worker(thread_pool_t *thread_pool) {

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
            if(thread_pool->workers_count > thread_pool->workers_limit_min) {
                // there is too many starving workers waiting for tasks, kill current worker
                remove_worker(thread_pool, pthread_self());
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
            } else {
                // min number of workers in thread pool, current worker cannot be killed
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

    if(size > max_size) {
        fprintf(stderr, "thread pool initialization failed, size is larger then max size!\n");
        return FAILURE;
    }

    // allocation of memory for thread pool and internal task queue
    *thread_pool = malloc(sizeof(thread_pool_t));
    task_queue_init( &((*thread_pool)->task_queue) );

    // allocation of memory for workers array
    (*thread_pool)->workers = (pthread_t *) malloc(sizeof(pthread_t)*max_size);
    (*thread_pool)->workers_limit_min = size;
    (*thread_pool)->workers_limit_max = max_size;
    (*thread_pool)->worker_ms_timeout = worker_ms_timeout;

    // init synchronization objects: mutex
    pthread_mutex_init(&mutex, NULL);

    // create worker threads
    pthread_attr_t pthread_attr;
    pthread_attr_init(&pthread_attr);

    for(int i=0; i<size; i++) {
        pthread_mutex_lock(&mutex);
        if( pthread_create(&((*thread_pool)->workers[i]), &pthread_attr, worker, (void *) *thread_pool) != 0 ) {
            fprintf(stderr, "pthread_create: failed to create worker thread.\n");
            return FAILURE;
        }
        (*thread_pool)->workers_count++;
        pthread_mutex_unlock(&mutex);
    }

    if((*thread_pool)->workers_count != size) {
        fprintf(stderr, "thread pool initialization failed, not created all worker threads!\n");
    }

    return SUCCESS;
}

void thread_pool_execute(thread_pool_t *thread_pool, task_t *task) {

}

void thread_pool_run(thread_pool_t *thread_pool, runner_t runner, runner_attr_t runner_attr, runner_res_handler_t runner_res_handler) {

}

void thread_pool_adjust_size(thread_pool_t *thread_pool) {

}

void thread_pool_free(thread_pool_t *thread_pool) {

}



