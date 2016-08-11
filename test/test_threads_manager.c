//
// Created by Michal Ziobro on 10/08/2016.
//

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "test_threads_manager.h"
#include "../concurrency/threads_manager.h"
#include "assertion.h"
#include "../common/array_helper.h"
#include "../common/time.h"

#define JOB_DURATION 5
#define MAX_LIMIT_OF_MANAGED_THREADS 5

runner_res_t runner(runner_attr_t runner_attr) {

    int *runner_num = (int *) runner_attr;
    sleep(JOB_DURATION);
    printf("managed thread: %p is running runner: %d.\n", pthread_self(), *runner_num);

    free(runner_num);
    return NULL;
}

/*******************************************************************/

static threads_manager_t *threads_manager;

static void test_create(void) {

    threads_manager_init(&threads_manager, MAX_LIMIT_OF_MANAGED_THREADS);
}

static void test_clean(void) {
    threads_manager_free(threads_manager);
    printf("*************************************************************\n\n");
}

static void test_threads_manager_count(void) {

    pthread_t *threads;
    size_t threads_count;

    test_create();

    printf("%s: \n", __func__);

    // create managed threads
    for (int i = 0; i < MAX_LIMIT_OF_MANAGED_THREADS; i++) {
        int *runner_num = malloc(sizeof(int));
        *runner_num = i;
        wait_for_thread(threads_manager, runner, runner_num);
    }

    // get managed threads
    threads = threads_manager_threads(threads_manager);
    threads_count = threads_manager_count(threads_manager);

    printf("managed threads - initial:\n");
    array_print( (const void **) threads, threads_count, pointer_printer);
    printf("managed threads count: %zu\n", threads_count);
    assert_equal_int(threads_count, MAX_LIMIT_OF_MANAGED_THREADS, "managed threads count - initial");

    sleep(JOB_DURATION+1);

    // get managed threads count again
    threads_count = threads_manager_count(threads_manager);

    printf("managed threads - after sleep:\n");
    array_print( (const void **) threads, threads_count, pointer_printer);
    printf("managed threads count: %zu\n", threads_count);
    assert_equal_int(threads_count, 0, "managed threads count - after sleep");

    test_clean();
}

static void test_wait_for_thread(void) {

    struct timeval time_0, time_1, time_2;
    int i, *runner_num;
    int diff_time_1, diff_time_2;

    test_create();

    printf("%s: \n", __func__);

    gettimeofday(&time_0, NULL);

    // create managed threads
    for (i = 0; i < MAX_LIMIT_OF_MANAGED_THREADS; i++) {
        runner_num = malloc(sizeof(int)); *runner_num = i;
        wait_for_thread(threads_manager, runner, runner_num);
    }

    gettimeofday(&time_1, NULL);

    // wait for new managed threads
    runner_num = malloc(sizeof(int)); *runner_num = i++;
    wait_for_thread(threads_manager, runner, runner_num);

    gettimeofday(&time_2, NULL);

    diff_time_1 = (timeval_difference(&time_1, &time_0)+500)/USEC_IN_MSEC; // [ms]
    diff_time_2 = (timeval_difference(&time_2, &time_1)+500)/USEC_IN_MSEC; // [ms]
    printf("time difference - creation under the limit: %d [ms]\n", diff_time_1);
    printf("time difference - creation beyond the limit: %d [ms]\n",diff_time_2);

    assert_equal_int((diff_time_1+500)/MSEC_IN_SEC, 0, "time difference - creation under the limit");
    assert_equal_int((diff_time_2+500)/MSEC_IN_SEC, JOB_DURATION, "time difference - creation beyond the limit");

    sleep(JOB_DURATION+1); // wait for the last runner to finish its job

    test_clean();
}

static void test_timed_wait_for_thread(void) {

    struct timeval time_0, time_1, time_2;
    int i, *runner_num;
    int diff_time_1, diff_time_2;
    pthread_t *threads;
    size_t threads_count;

    test_create();

    printf("%s: \n", __func__);

    gettimeofday(&time_0, NULL);
    threads = threads_manager_threads(threads_manager);
    threads_count = threads_manager_count(threads_manager);

    printf("managed threads - initial:\n");
    array_print( (const void **) threads, threads_count, pointer_printer);
    printf("managed threads count: %zu\n", threads_count);
    assert_equal_int(threads_count, 0, "managed threads count - initial");

    // create managed threads
    for (i = 0; i < MAX_LIMIT_OF_MANAGED_THREADS; i++) {
        runner_num = malloc(sizeof(int)); *runner_num = i;
        wait_for_thread(threads_manager, runner, runner_num);
    }

    gettimeofday(&time_1, NULL);
    threads = threads_manager_threads(threads_manager);
    threads_count = threads_manager_count(threads_manager);

    printf("managed threads - first creation:\n");
    array_print( (const void **) threads, threads_count, pointer_printer);
    printf("managed threads count: %zu\n", threads_count);
    assert_equal_int(threads_count, MAX_LIMIT_OF_MANAGED_THREADS, "managed threads count - first creation");

    // timed wait for new managed threads
    // 1st thread -> half of job duration
    runner_num = malloc(sizeof(int)); *runner_num = i++;
    timed_wait_for_thread(threads_manager, JOB_DURATION*MSEC_IN_SEC/2 /* [ms] */, runner, runner_num);
    // 2nd thread -> more than job duration
    runner_num = malloc(sizeof(int)); *runner_num = i++;
    timed_wait_for_thread(threads_manager, JOB_DURATION*MSEC_IN_SEC + 1 /* [ms] */, runner, runner_num);

    gettimeofday(&time_2, NULL);
    sleep(1); // wait a little more until all threads finish
    threads = threads_manager_threads(threads_manager);
    threads_count = threads_manager_count(threads_manager);

    printf("managed threads - additional creation:\n");
    array_print( (const void **) threads, threads_count, pointer_printer);
    printf("managed threads count: %zu\n", threads_count);
    assert_equal_int(threads_count, 1, "managed threads count - additional creation");

    diff_time_1 = (timeval_difference(&time_1, &time_0)+500)/USEC_IN_MSEC; // [ms]
    diff_time_2 = (timeval_difference(&time_2, &time_1)+500)/USEC_IN_MSEC; // [ms]
    printf("time difference - creation under the limit: %d [ms]\n", diff_time_1);
    printf("time difference - creation beyond the limit: %d [ms]\n",diff_time_2);

    assert_equal_int((diff_time_1+500)/MSEC_IN_SEC, 0, "time difference - creation under the limit");
    assert_equal_int((diff_time_2+500)/MSEC_IN_SEC, JOB_DURATION, "time difference - creation beyond the limit");

    sleep(JOB_DURATION+1); // wait for the last runner to finish its job

    test_clean();
}

static void run_tests(void) {
    test_threads_manager_count();
    test_wait_for_thread();
    test_timed_wait_for_thread();
}

test_threads_manager_t test_threads_manager = { .run_tests = run_tests };