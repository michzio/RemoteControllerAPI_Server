//
// Created by Michal Ziobro on 01/08/2016.
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../concurrency/thread_pool/thread_pool.h"
#include "test_thread_pool.h"

// short task runner
static runner_res_t short_task_runner(runner_attr_t arg) {

    sleep(8-*((int *)arg)); // 1 second
    printf("worker thread: %p running short task: %d...\n", pthread_self(), *((int *)arg));

    return arg;
}
static void short_task_res_handler(runner_res_t arg) {

    printf("worker thread: %p handling results of task runner for short task: %d...\n", pthread_self(), *((int *)arg));
    free(arg);
}

// long task runner
static runner_res_t long_task_runner(runner_attr_t arg) {

    printf("worker thread: %p running long task: %d...\n", pthread_self(), *((int *) arg));

    return arg;
}
static void long_task_res_handler(runner_res_t arg) {

    printf("worker thread: %p handling results of task runner for long task: %d...\n", pthread_self(), *((int *)arg));
    free(arg);
}
/*******************************************************************************************************************/

// thread pool object
static thread_pool_t *thread_pool;

static void test_create(void) {

    thread_pool_init_default(&thread_pool);
}

static void test_clean(void) {

    thread_pool_force_free(thread_pool);
}

// tests
static void test_thread_pool_execute(void) {
    test_create();

    // execute short tasks
    // number of tasks equal number of worker threads
    for(int i=0; i<8; i++) {

        int *task_num = malloc(sizeof(int));
        *task_num = i;

        task_t *task;
        task_init(&task);
        task_fill(task, short_task_runner, task_num, short_task_res_handler);

        thread_pool_execute(thread_pool, task);
    }

    sleep(4);

    test_clean();
}

static void test_thread_pool_run(void) {
    test_create();

    test_clean();
}

static void test_thread_pool_adjust_size(void) {
    test_create();

    test_clean();
}

static void test_thread_pool_pause_and_resume(void) {
    test_create();

    test_clean();
}

static void test_thread_pool_shutdown(void) {
    test_create();

    // custom clean of thread pool via shutdown gracefully

}

static void test_thread_pool_force_free(void) {
    test_create();

    // custom clean of thread pool via force free
}

static void run_tests(void) {
    test_thread_pool_execute();
   // test_thread_pool_run();
   // test_thread_pool_adjust_size();
   // test_thread_pool_pause_and_resume();
   // test_thread_pool_shutdown();
   // test_thread_pool_force_free();
}

test_thread_pool_t test_thread_pool = { .run_tests = run_tests };