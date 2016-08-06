//
// Created by Michal Ziobro on 28/07/2016.
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
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
    runner_res_handler_t runner_res_handler;
};

// synchronization of access to queue
static pthread_mutex_t mutex;
static pthread_cond_t conditional_variable;

// task allocator handlers
static void task_allocate_handler(void **data_store, void *data, size_t data_size) {

    // copy task to data store
    *data_store = (task_t *) malloc(data_size);
    memcpy(*data_store, (task_t *) data, data_size);
}

static void task_deallocate_handler(void **data_store) {

    free((task_t *) *data_store);
}

static void cleanup_unlock_mutex(void *p)
{
    pthread_mutex_unlock(p);
}

// task queue operations

void task_queue_init(task_queue_t **task_queue) {

    // allocation of memory for task queue and internal fifo queue
    *task_queue = malloc(sizeof(task_queue_t));

    allocator_t *task_allocator;
    allocator_init(&task_allocator, task_allocate_handler, task_deallocate_handler);

    fifo_init( &((*task_queue)->fifo), task_allocator );

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
    fifo_enqueue(task_queue->fifo, task, sizeof(task_t));
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

    task_t *task;

    // 1. take mutex
    pthread_mutex_lock(&mutex);
    pthread_cleanup_push(cleanup_unlock_mutex, (void *) &mutex); // cleanup handler for the case of thread cancellation

    // 2. while task queue is empty
    while (task_queue->task_count == 0) {
        printf("thread: %p is waiting on the queue for new tasks.\n", pthread_self());
        // 3. wait for new tasks on conditional variable (mutex released, and if signaled granted again)
        pthread_cond_wait(&conditional_variable, &mutex);
    }

    // 4. dequeue task
    task = (task_t *) fifo_dequeue(task_queue->fifo, NULL);
    task_queue->task_count--; // decrement num of tasks

    printf("thread: %p has taken task from the queue.\n", pthread_self());

    // 5. release mutex
    pthread_cleanup_pop(0);
    pthread_mutex_unlock(&mutex);

    return task;
}

static void set_timespec_from_timeout(struct timespec *timespec, int ms_timeout) {

    struct timeval timeval;
    gettimeofday(&timeval, NULL);

    timespec->tv_sec = time(NULL) + ms_timeout/1000;
    timespec->tv_nsec = timeval.tv_usec * 1000 + 1000 * 1000 * (ms_timeout % 1000);
    timespec->tv_sec += timespec->tv_nsec / (1000 * 1000 * 1000);
    timespec->tv_nsec %= (1000 * 1000 * 1000);
}

task_t *dequeue_task_timed(task_queue_t *task_queue, int ms_timeout) {

    task_t *task = NULL;
    struct timespec timespec;
    set_timespec_from_timeout(&timespec, ms_timeout);

    // 1. take mutex
    pthread_mutex_lock(&mutex);
    pthread_cleanup_push(cleanup_unlock_mutex, (void *) &mutex); // cleanup handler for the case of thread cancellation

    // 2. while task queue is empty
    while (task_queue->task_count == 0) {
        printf("thread: %p is timed waiting on the queue for new tasks.\n", pthread_self());
        // 3. wait for new tasks on conditional variable (mutex released, and if signaled granted again)
        if(pthread_cond_timedwait(&conditional_variable, &mutex, &timespec) == ETIMEDOUT) {
            fprintf(stderr, "thread: %p timed out.\n", pthread_self());
            goto UNLOCK_AND_RETURN;
        }
    }

    // 4. dequeue task
    task = fifo_dequeue(task_queue->fifo, NULL);
    task_queue->task_count--; // decrement num of tasks

    printf("thread: %p has taken task from the queue.\n", pthread_self());

    UNLOCK_AND_RETURN:
    // 5. release mutex
    pthread_cleanup_pop(0);
    pthread_mutex_unlock(&mutex);

    return task;
}

int task_queue_count(task_queue_t *task_queue) {
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
    (*task)->runner_res_handler = NULL;
}

/**
 * function fills task with runner, runner attributes and runner result handler
 */
void task_fill(task_t *task, runner_t runner, runner_attr_t runner_attr, runner_res_handler_t runner_res_handler) {

    if(task == NULL)  {
        fprintf(stderr, "task_fill: task argument is NULL!\n");
        return;
    }

    task->runner = runner;
    task->runner_attr = runner_attr;
    task->runner_res_handler = runner_res_handler;
}

/**
 * function executes given task using it's
 * runner (function pointer) and by passing
 * runner_attr to it. It returns results
 * through runner_res field in task structure.
 */
void task_run(task_t *task) {
    task->runner_res = task->runner(task->runner_attr);
    // handle runner results if handler is defined
    if(task->runner_res_handler != NULL)
        task->runner_res_handler(task->runner_res);
}

void task_free(task_t *task) {

    free(task);
    task = NULL;
}