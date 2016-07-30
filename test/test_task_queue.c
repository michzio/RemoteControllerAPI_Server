//
// Created by Michal Ziobro on 30/07/2016.
//

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "test_task_queue.h"
#include "../concurrency/thread_pool/task_queue.h"

#define NUM_WORKERS 10

static task_queue_t *task_queue;
static pthread_t worker[NUM_WORKERS];

static void test_create(void) {

    task_queue_init(&task_queue);
}

static void test_clean(void) {

    for(int i=0; i<NUM_WORKERS; i++)
        pthread_join(worker[i], NULL);

    task_queue_free(task_queue);

}

static void *test_enqueue(void *arg) {

    printf("test_enqueue(): %p\n", pthread_self());

    task_t *task;
    task_init(&task);

    enqueue_task(task_queue, task);
}

static void *test_dequeue(void *arg) {

    printf("test_dequeue(): %p\n", pthread_self());

    task_t *task = dequeue_task(task_queue);

    task_free(task);
}

static void run_tests(void) {
    test_create(); // create task_queue

    printf("test_task_queue:\n");
    // create random number of test threads that enqueue and dequeue tasks
    srand(time(NULL));
    for(int i=0; i<NUM_WORKERS; i++) {
        int n = rand() % 2;
        if (n) {
            pthread_create(&(worker[i]), NULL, test_dequeue, NULL); // run dequeueing thread
        } else {
            pthread_create(&(worker[i]), NULL, test_enqueue, NULL); // run enqueueing thread
        }
    }

    test_clean(); // destroy task_queue
}

test_task_queue_t test_task_queue = { .run_tests = run_tests };