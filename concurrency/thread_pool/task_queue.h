//
// Created by Michal Ziobro on 28/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_TASK_QUEUE_H
#define REMOTECONTROLLERAPI_SERVER_TASK_QUEUE_H

// TASK QUEUE
struct task_queue;
typedef struct task_queue task_queue_t;

// TASK
struct task;
typedef struct task task_t;

typedef void * runner_attr_t;
typedef void * runner_res_t;
typedef runner_res_t (*runner_t)(runner_attr_t);

// task queue operations
void task_queue_init(task_queue_t **task_queue);
void enqueue_task(task_queue_t *task_queue, task_t *task);
task_t *dequeue_task(task_queue_t *task_queue);
int task_count(task_queue_t *task_queue);
void task_queue_free(task_queue_t *task_queue);

// task operations
void task_init(task_t **task);
void task_run(task_t *task);
void task_free(task_t *task);

#endif //REMOTECONTROLLERAPI_SERVER_TASK_QUEUE_H
