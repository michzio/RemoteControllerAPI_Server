//
// Created by Michal Ziobro on 01/08/2016.
//

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "../concurrency/thread_pool/thread_pool.h"
#include "test_thread_pool.h"
#include "assertion.h"
#include "../common/numbers.h"

#define DEFAULT_MIN_SIZE 8
#define DEFAULT_MAX_SIZE 16
#define DEFAULT_TIMEOUT 1000

static int runner_counter;
static int runner_sum;
static pthread_mutex_t runner_mutex;

// task runner
static runner_res_t task_runner(runner_attr_t arg) {

    int sleep_timeout = DEFAULT_MIN_SIZE-*((int *)arg);
    sleep(sleep_timeout); // [seconds]
    printf("worker thread: %p running task: %d...\n", pthread_self(), *((int *)arg));

    return arg;
}
static void task_res_handler(runner_res_t arg) {

    pthread_mutex_lock(&runner_mutex);
    runner_counter++;
    runner_sum  += *((int *) arg);
    pthread_mutex_unlock(&runner_mutex);
    printf("worker thread: %p handling results of task runner for task: %d...\n", pthread_self(), *((int *)arg));
    free(arg);
}

/*******************************************************************************************************************/

// thread pool object
static thread_pool_t *thread_pool;

static void test_create(void) {

    runner_counter = 0;
    runner_sum = 0;
    pthread_mutex_init(&runner_mutex, NULL);
    thread_pool_init(&thread_pool, DEFAULT_MIN_SIZE, DEFAULT_MAX_SIZE, DEFAULT_TIMEOUT);
}

static void test_clean(void) {

    thread_pool_force_free(thread_pool);
    pthread_mutex_destroy(&runner_mutex);
}

// tests
static void test_thread_pool_execute(void) {

    int sleep_timeout;
    sleep_timeout = range_rand(0, DEFAULT_MIN_SIZE);

    printf("%s: \n", __func__);

    test_create();

    // execute tasks, number of tasks equal number of worker threads
    for(int i=0; i<DEFAULT_MIN_SIZE; i++) {

        int *task_num = malloc(sizeof(int));
        *task_num = i;

        task_t *task;
        task_init(&task);
        task_fill(task, task_runner, task_num, task_res_handler);

        thread_pool_execute(thread_pool, task);
    }

    sleep(sleep_timeout);
    test_clean();

    int sum = 0;
    for(int i=(DEFAULT_MIN_SIZE-sleep_timeout); i<DEFAULT_MIN_SIZE; i++) sum += i;

    assert_equal_int(runner_counter, sleep_timeout, "test_thread_pool_execute - runner counter");
    printf("runner counter: %d, sleep timeout: %d\n", runner_counter, sleep_timeout);
    assert_equal_int(runner_sum, sum, "test_thread_pool_execute - runner sum");
    printf("runner sum: %d, sum: %d\n", runner_sum, sum);
}

static void test_thread_pool_run(void) {

    int sleep_timeout;
    sleep_timeout = range_rand(0, DEFAULT_MIN_SIZE);

    printf("%s: \n", __func__);

    test_create();

    // run task runners with attributes and result handlers
    // number of runners launched equal number of worker threads
    for(int i=0; i<DEFAULT_MIN_SIZE; i++) {

        int *task_num = malloc(sizeof(int));
        *task_num = i;

        thread_pool_run(thread_pool, task_runner, task_num, task_res_handler);
    }

    sleep(sleep_timeout);
    test_clean();

    int sum = 0;
    for(int i=(DEFAULT_MIN_SIZE-sleep_timeout); i<DEFAULT_MIN_SIZE; i++) sum +=i;

    assert_equal_int(runner_counter, sleep_timeout, "test_thread_pool_run - runner counter");
    printf("runner counter: %d, sleep timeout: %d\n", runner_counter, sleep_timeout);
    assert_equal_int(runner_sum, sum, "test_thread_pool_run - runner sum");
    printf("runner sum: %d, sum: %d\n", runner_sum, sum);
}

static void test_thread_pool_adjust_size(void) {

    int workers_count = 0;

    printf("%s: \n", __func__);

    test_create();

    printf("workers count - initial: %d\n", thread_pool_workers_count(thread_pool));

    // run more tasks than workers
    for(int i=0; i<DEFAULT_MAX_SIZE; i++) {
        int *task_attr = malloc(sizeof(int));
        *task_attr = 0;
        thread_pool_run(thread_pool, task_runner, task_attr, task_res_handler);
    }

    thread_pool_adjust_size(thread_pool);
    workers_count = thread_pool_workers_count(thread_pool);
    printf("workers count - first adjust: %d\n", workers_count);
    assert_greater_than(workers_count, DEFAULT_MIN_SIZE, "test_thread_pool_adjust_size - first adjust");

    thread_pool_set_size(thread_pool, DEFAULT_MAX_SIZE+1, 2*DEFAULT_MAX_SIZE);
    thread_pool_adjust_size(thread_pool);
    workers_count = thread_pool_workers_count(thread_pool);
    printf("workers count - second adjust: %d\n", workers_count);
    assert_greater_than(workers_count, DEFAULT_MAX_SIZE, "test_thread_pool_adjust_size - second adjust");

    test_clean();
}

static void test_thread_pool_workers_self_termination(void) {

    int workers_count = 0;

    // create more workers than needed to execute all tasks
    test_create();

    workers_count = thread_pool_workers_count(thread_pool);
    printf("workers count - before self termination: %d\n", workers_count);

    for(int i=0; i<DEFAULT_MIN_SIZE/2; i++) {
        int *task_num = malloc(sizeof(int));
        *task_num = 0;
        thread_pool_run(thread_pool, task_runner, task_num, task_res_handler);
    }

    // number of workers have to be greater than minimal limit
    thread_pool_set_size(thread_pool, DEFAULT_MIN_SIZE/2, DEFAULT_MAX_SIZE);

    sleep(1); // wait 1 sec, as it may take up to 1 sec worker threads to self terminate

    workers_count = thread_pool_workers_count(thread_pool);
    assert_equal_int(workers_count, DEFAULT_MIN_SIZE/2, "test_thread_pool_workers_self_termination");
    printf("workers count - after self termination: %d\n", workers_count);

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
    srand(time(NULL));
    //test_thread_pool_execute();
    //test_thread_pool_run();
    test_thread_pool_adjust_size();
    //test_thread_pool_workers_self_termination();
    //test_thread_pool_pause_and_resume();
    //test_thread_pool_shutdown();
    //test_thread_pool_force_free();
}

test_thread_pool_t test_thread_pool = { .run_tests = run_tests };