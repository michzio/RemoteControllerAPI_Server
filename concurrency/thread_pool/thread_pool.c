//
// Created by Michal Ziobro on 28/07/2016.
//

#include <ntsid.h>
#include "thread_pool.h"
#include "task_queue.h"

const size_t default_pool_size = 10;    // default num of workers if not specified by user
const size_t max_pool_size = 20;        // max num of workers if not specified by user

struct thread_pool {
    task_queue_t *task_queue;   // shared queue storing tasks to be run by workers
    pthread_t *workers;         // worker threads running tasks from queue
    int workers_count;          // number of workers
    int workers_limit;          // max number of workers in the pool
};

// thread pool operations

void thread_pool_init(thread_pool_t **thread_pool, const size_t size, const size_t max_size) {

}

void thread_pool_execute(thread_pool_t *thread_pool, task_t *task) {

}

void thread_pool_run(thread_pool_t *thread_pool, runner_t runner, runner_attr_t runner_attr, runner_res_handler_t runner_res_handler) {

}

void thread_pool_adjust_size(thread_pool_t *thread_pool) {

}

void thread_pool_free(thread_pool_t *thread_pool) {

}



