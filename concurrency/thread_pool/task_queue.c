//
// Created by Michal Ziobro on 28/07/2016.
//

#include <pthread.h>
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

/**
 * function enqueueing task is implemented
 * as producer from producer-consumer model.
 * It uses mutex to get exclusive access to
 * task queue. If task queue is empty it
 * signals waiting worker threads about
 * addition of new task to the queue.
 */
void enqueue_task(task_queue_t *task_queue, task_t *task) {

    // 1. take mutex
    pthread_mutex_lock(&mutex);

    // 2. is task queue empty?
    if (task_queue->task_count == 0)
        // 3. if so signal waiting threads (workers) that new task has been enqueued
        pthread_cond_signal(&conditional_variable);

    // 4. enqueue new task
    fifo_enqueue(task_queue->fifo, task, sizeof(task));
    task_queue->task_count++; // increment num of tasks

    // 5. release mutex
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
    while (task_queue->task_count == 0)
        // 3. wait for new tasks on conditional variable (mutex released, and if signaled granted again)
        pthread_cond_wait(&conditional_variable, &mutex);

    // 4. dequeue task
    task_t *task = fifo_dequeue(task_queue->fifo, NULL);
    task_queue->task_count--; // decrement num of tasks

    // 5. release mutex
    pthread_mutex_unlock(&mutex);
}