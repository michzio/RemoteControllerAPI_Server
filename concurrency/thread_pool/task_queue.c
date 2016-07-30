//
// Created by Michal Ziobro on 28/07/2016.
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "task_queue.h"
#include "../../common/fifo_queue.h"

// TASK QUEUE
struct task_queue {
    fifo_queue_t *fifo;
    int task_count;
};

// TASK
struct task {
    runner_t runner;
    runner_attr_t runner_attr;
    runner_res_t runner_res;
};

// synchronization of access to queue
static pthread_mutex_t mutex;
static pthread_cond_t conditional_variable;

// task queue operations

void task_queue_init(task_queue_t **task_queue) {

    // allocation of memory for task queue and internal fifo queue
    *task_queue = malloc(sizeof(task_queue_t));
    fifo_init( &((*task_queue)->fifo) );

    // set initial num of tasks in the queue
    (*task_queue)->task_count = 0;

    // init synchronization objects: mutex, conditional variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&conditional_variable, NULL);
}

/**
 * function enqueueing task is implemented
 * as producer from producer-consumer model.
 * It uses mutex to get exclusive access to
 * the task queue. It signals waiting worker threads
 * about addition of new task to the queue.
 */
void enqueue_task(task_queue_t *task_queue, task_t *task) {

    // 1. take mutex
    pthread_mutex_lock(&mutex);

    // 2. enqueue new task
    fifo_enqueue(task_queue->fifo, task, sizeof(task));
    task_queue->task_count++; // increment num of tasks

    printf("thread: %p has added task to the queue.\n", pthread_self());

    // 3. signal waiting threads (workers) that new task has been enqueued
    pthread_cond_signal(&conditional_variable);

    // 4. release mutex
    pthread_mutex_unlock(&mutex);
}

/**
 * function dequeueing task is implemented
 * as consumer from producer-consumer model.
 * It uses mutex to get exclusive access to
 * task queue. If task queue is empty it
 * stops worker thread and add it to waiting
 * queue on conditional variable until new
 * task will be added to the task queue.
 * When it gets signaled about a new task
 * than it execute task dequeueing and after
 * it releases mutex.
 */
task_t *dequeue_task(task_queue_t *task_queue) {

    // 1. take mutex
    pthread_mutex_lock(&mutex);

    // 2. while task queue is empty
    while (task_queue->task_count == 0) {
        printf("thread: %p is waiting on the queue for new tasks.\n", pthread_self());
        // 3. wait for new tasks on conditional variable (mutex released, and if signaled granted again)
        pthread_cond_wait(&conditional_variable, &mutex);
    }

    // 4. dequeue task
    task_t *task = fifo_dequeue(task_queue->fifo, NULL);
    task_queue->task_count--; // decrement num of tasks

    printf("thread: %p has taken task from the queue.\n", pthread_self());

    // 5. release mutex
    pthread_mutex_unlock(&mutex);

    return task;
}

int task_count(task_queue_t *task_queue) {
    return task_queue->task_count;
}

void task_queue_free(task_queue_t *task_queue) {

    // deallocate internal fifo queue
    fifo_free(task_queue->fifo);
    // reset task queue counter
    task_queue->task_count = 0;
    // free task queue
    free(task_queue);
    task_queue = NULL;

    // destroy mutex and conditional variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&conditional_variable);
}

// task operations

void task_init(task_t **task) {

    *task = malloc(sizeof(task_t));
    (*task)->runner_attr = NULL;
    (*task)->runner = NULL;
    (*task)->runner_res = NULL;
}

/**
 * function executes given task using it's
 * runner (function pointer) and by passing
 * runner_attr to it. It returns results
 * through runner_res field in task structure.
 */
void task_run(task_t *task) {
    task->runner_res = task->runner(task->runner_attr);
}

void task_free(task_t *task) {

    free(task);
    task = NULL;
}