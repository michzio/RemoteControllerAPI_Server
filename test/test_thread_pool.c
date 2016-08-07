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
    sleep_timeout = (sleep_timeout < 0) ? 0 : sleep_timeout;
    sleep(sleep_timeout); // [seconds]
    printf("worker thread: %p running task: %d...\n", pthread_self(), *((int *)arg));

    return arg;
}

static runner_res_t long_task_runner(runner_attr_t arg) {

    sleep(8); // [seconds]
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
        *task_attr = i;
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

    printf("%s: \n", __func__);

    // create more workers than needed to execute all tasks
    test_create();

    workers_count = thread_pool_workers_count(thread_pool);
    printf("workers count - before self termination: %d\n", workers_count);

    for(int i=0; i<DEFAULT_MIN_SIZE/2; i++) {
        int *task_num = malloc(sizeof(int));
        *task_num = i;
        thread_pool_run(thread_pool, task_runner, task_num, task_res_handler);
    }

    // number of workers have to be greater than minimal limit
    thread_pool_set_size(thread_pool, DEFAULT_MIN_SIZE/2, DEFAULT_MAX_SIZE);

    sleep(2); // wait 2 sec, as it may take up to 1 sec worker threads to self terminate

    workers_count = thread_pool_workers_count(thread_pool);
    assert_equal_int(workers_count, DEFAULT_MIN_SIZE/2, "test_thread_pool_workers_self_termination");
    printf("workers count - after self termination: %d\n", workers_count);

    test_clean();
}

static void test_thread_pool_pause_and_resume(void) {

    int after_pause_count;
    int after_pause_sum;
    int before_resume_one_count;
    int before_resume_one_sum;
    int before_resume_count;
    int before_resume_sum;

    printf("%s: \n", __func__);

    test_create();

    for(int i=0; i<DEFAULT_MIN_SIZE; i++) {
        int *task_num = malloc(sizeof(int));
        *task_num = i;
        thread_pool_run(thread_pool, task_runner, task_num, task_res_handler);
    }

    sleep(DEFAULT_MIN_SIZE/2);

    thread_pool_pause(thread_pool);
    /*********************************************
     * here all running workers should be on hold
     */
    after_pause_count = runner_counter;
    after_pause_sum = runner_sum;

    printf("*********************** SLEEPING **************************\n");
    sleep(10); // wait 10 seconds until resume

    before_resume_one_count = runner_counter;
    before_resume_one_sum = runner_sum;
    /*
     * resuming workers...
     **********************************************/
    thread_pool_resume_one(thread_pool);
    printf("*********************** SLEEPING **************************\n");
    sleep(5);
    before_resume_count = runner_counter;
    before_resume_sum = runner_sum;
    thread_pool_resume(thread_pool);

    sleep(1);

    test_clean();

    // print test results
    assert_equal_int(after_pause_count, before_resume_one_count, "test_thread_pool_pause_and_resume - runners counter (one resumed)");
    printf("runners counter just after pause: %d\n", after_pause_count);
    printf("runners counter just before one resume: %d\n", before_resume_one_count);
    assert_equal_int(after_pause_sum, before_resume_one_sum, "test_thread_pool_pause_and_resume - runners sum (one resumed)");
    printf("runners sum just after pause: %d\n", after_pause_sum);
    printf("runners sum just before one resume: %d\n", before_resume_one_sum);
    assert_in_range( (before_resume_count - after_pause_count), 0, 1, "test_thread_pool_pause_and_resume - runners counter (all resumed)");
    printf("runners counter just after pause: %d\n", after_pause_count);
    printf("runners counter just before one resume: %d\n", before_resume_one_count);
    printf("runners counter just before all resume: %d\n", before_resume_count);
}

void test_thread_pool_set_timeout(void) {

    int workers_count = 0;
    int old_workers_count = 0;

    printf("%s: \n", __func__);

    test_create();

    workers_count = thread_pool_workers_count(thread_pool);
    printf("workers count - initial: %d\n", workers_count);
    printf("old workers count: %d, new workers count: %d\n", old_workers_count, workers_count);

    // 0ms timeout cause threads never timeout, and never die!
    thread_pool_set_timeout(thread_pool, 0);

    old_workers_count = workers_count;
    workers_count = thread_pool_workers_count(thread_pool);
    assert_equal_int(old_workers_count, workers_count, "test_thread_pool_set_timeout - workers count (timeout=0,count=min)");
    printf("old workers count: %d, new workers count: %d\n", old_workers_count, workers_count);

    sleep(2);

    // now normally starving threads will be self terminating
    thread_pool_set_size(thread_pool, 0, DEFAULT_MAX_SIZE);

    sleep(10);

    old_workers_count = workers_count;
    workers_count = thread_pool_workers_count(thread_pool);
    assert_equal_int(old_workers_count, workers_count, "test_thread_pool_set_timeout - workers count (timeout=0, min=0, count>min)");
    printf("old workers count: %d, new workers count: %d\n", old_workers_count, workers_count);

    // 10ms timeout cause fast dying of threads
    thread_pool_set_timeout(thread_pool, 10);

    // add tasks to unblock threads infinitely waiting on conditional variable
    for(int i=0; i<DEFAULT_MIN_SIZE; i++) {
        int *task_num = malloc(sizeof(int));
        *task_num = i;
        thread_pool_run(thread_pool, task_runner, task_num, task_res_handler);
    }

    sleep(10);

    old_workers_count = workers_count;
    workers_count = thread_pool_workers_count(thread_pool);
    assert_equal_int(0, workers_count, "test_thread_pool_set_timeout - workers count (timeout=10, min=0, count>min)");
    printf("old workers count: %d, new workers count: %d\n", old_workers_count, workers_count);

    test_clean();
}

static void test_thread_pool_workers_count(void) {

    int workers_count = 0;

    test_create();

    workers_count = thread_pool_workers_count(thread_pool);
    assert_equal_int(DEFAULT_MIN_SIZE, workers_count, "test_thread_pool_workers_count - initial");
    printf("initial workers count: %d\n", workers_count);

    // resize thread_pool
    thread_pool_set_size(thread_pool, DEFAULT_MAX_SIZE, DEFAULT_MAX_SIZE);
    thread_pool_adjust_size(thread_pool);

    workers_count = thread_pool_workers_count(thread_pool);
    assert_equal_int(DEFAULT_MAX_SIZE, workers_count, "test_thread_pool_workers_count - resized");
    printf("resized workers count: %d\n", workers_count);

    test_clean();
}

static void test_thread_pool_workers_timeout(void) {

    int workers_timeout = -1;

    test_create();

    workers_timeout = thread_pool_workers_timeout(thread_pool);
    assert_equal_int(1000 /* [ms] */, workers_timeout, "test_thread_pool_workers_timeout - initial");
    printf("initial workers timeout: %d\n", workers_timeout);

    // change workers timeout in thread pool
    thread_pool_set_timeout(thread_pool, 0 /* [ms] */);

    workers_timeout = thread_pool_workers_timeout(thread_pool);
    assert_equal_int(0 /* [ms] */, workers_timeout, "test_thread_pool_workers_timeout - changed");
    printf("changed workers timeout: %d\n ", workers_timeout);

    test_clean();
}

static void test_thread_pool_shutdown(void) {
    test_create();

    for(int i=0; i<DEFAULT_MIN_SIZE; i++) {
        int *task_num = malloc(sizeof(int));
        *task_num = i;
        thread_pool_run(thread_pool, long_task_runner, task_num, task_res_handler);
    }

    printf("******************* START SHUTTING DOWN ************************\n");

    // custom clean of thread pool via shutdown gracefully
    thread_pool_shutdown(thread_pool);
    pthread_mutex_destroy(&runner_mutex);

    printf("******************* SHUTTED DOWN *******************************\n");

    // long tasks should be able to complete before thread pool shuts down gracefully. test it.
    assert_equal_int(DEFAULT_MIN_SIZE, runner_counter, "test_thread_pool_shutdown");
    printf("runner counter: %d\n", runner_counter);
    printf("runner sum: %d\n", runner_sum);
}

static void test_thread_pool_force_free(void) {
    test_create();

    for(int i=0; i<DEFAULT_MIN_SIZE; i++) {
        int *task_num = malloc(sizeof(int));
        *task_num = i;
        thread_pool_run(thread_pool, long_task_runner, task_num, task_res_handler);
    }

    // custom clean of thread pool via force free
    thread_pool_force_free(thread_pool);
    pthread_mutex_destroy(&runner_mutex);

    // long tasks shouldn't be completed before thread pool is forcefully freed. test it.
    assert_equal_int(0, runner_counter, "test_thread_pool_force_free");
    printf("runner counter: %d\n", runner_counter);
    printf("runner sum: %d\n", runner_sum);

}

static void run_tests(void) {
    srand(time(NULL));
    test_thread_pool_execute();
    test_thread_pool_run();
    test_thread_pool_adjust_size();
    test_thread_pool_workers_self_termination();
    test_thread_pool_pause_and_resume();
    test_thread_pool_set_timeout();
    test_thread_pool_workers_count();
    test_thread_pool_workers_timeout();
    test_thread_pool_shutdown();
    test_thread_pool_force_free();
}

test_thread_pool_t test_thread_pool = { .run_tests = run_tests };