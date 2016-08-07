//
// Created by Michal Ziobro on 28/07/2016.
//

/**
 * The threads in the pool take tasks off the queue, perform them, then return to the queue for their next task.
 */

#ifndef REMOTECONTROLLERAPI_SERVER_THREAD_POOL_H
#define REMOTECONTROLLERAPI_SERVER_THREAD_POOL_H

#include "task_queue.h"
#include "../../common/address_helper.h"

struct thread_pool;
typedef struct thread_pool thread_pool_t;

// thread pool operations
result_t thread_pool_init_default(thread_pool_t **thread_pool);
result_t thread_pool_init(thread_pool_t **thread_pool, const size_t size, const size_t max_size, const int worker_ms_timeout);
void thread_pool_execute(thread_pool_t *thread_pool, task_t *task);
void thread_pool_run(thread_pool_t *thread_pool, runner_t runner, runner_attr_t runner_attr, runner_res_handler_t runner_res_handler);
result_t thread_pool_adjust_size(thread_pool_t *thread_pool);
void thread_pool_pause(thread_pool_t *thread_pool);
void thread_pool_resume(thread_pool_t *thread_pool);
void thread_pool_resume_one(thread_pool_t *thread_pool);
void thread_pool_set_size(thread_pool_t *thread_pool, const size_t min_size, const size_t max_size);
void thread_pool_set_timeout(thread_pool_t *thread_pool, const int worker_ms_timeout);
int thread_pool_workers_count(thread_pool_t *thread_pool);
int thread_pool_workers_timeout(thread_pool_t *thread_pool);
void thread_pool_shutdown(thread_pool_t *thread_pool);
void thread_pool_force_free(thread_pool_t *thread_pool);

#endif //REMOTECONTROLLERAPI_SERVER_THREAD_POOL_H
