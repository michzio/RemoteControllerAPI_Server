//
// Created by Michal Ziobro on 29/07/2016.
//


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../common/fifo_queue.h"
#include "test_fifo_queue.h"

static fifo_queue_t *fifo;

static void test_create(void) {
    fifo_init(&fifo, NULL);
}

static void test_create_with_allocator(void) {

    allocator_t *string_allocator;
    allocator_init(&string_allocator, string_allocate_handler, string_deallocate_handler);

    fifo_init(&fifo, string_allocator);

}

static void test_clean(void) {
    fifo_free(fifo);
}

static void test_clean_with_allocator(void) {
    fifo_free(fifo);
}

static void test_enqueue_and_dequeue(void) {
    test_create();

    printf("%s:\n", __func__);
    for(int i=0; i<10; i++) {
        char *data = malloc(256);
        sprintf(data, "data %d", i);
        fifo_enqueue(fifo, data, sizeof(data));
        printf("enqueue: %s, ", data);
    }
    printf("\n");
    for(int i=0; i<5; i++) {
        char *data = fifo_dequeue(fifo, NULL);
        printf("dequeue: %s, ", data);
        free(data);
    }
    printf("\n");
    for(int i=10; i<13; i++) {
        char *data = malloc(256);
        sprintf(data, "data %d", i);
        fifo_enqueue(fifo, data, sizeof(data));
        printf("enqueue: %s, ", data);

    }
    printf("\n");
    for(int i=5; i<13; i++) {
        char *data = fifo_dequeue(fifo, NULL);
        printf("dequeue: %s, ", data);
        free(data);
    }
    printf("\n");

    test_clean();
}

static void test_enqueue_and_dequeue_with_allocator(void) {
    test_create_with_allocator();

    printf("%s:\n", __func__);
    for(int i=0; i<10; i++) {
        char *data = malloc(256);
        sprintf(data, "data %d", i);
        fifo_enqueue(fifo, data, sizeof(data));
        printf("enqueue: %s, ", data);
    }
    printf("\n");
    for(int i=0; i<5; i++) {
        char *data = fifo_dequeue(fifo, NULL);
        printf("dequeue: %s, ", data);
        free(data);
    }
    printf("\n");
    for(int i=10; i<13; i++) {
        char *data = malloc(256);
        sprintf(data, "data %d", i);
        fifo_enqueue(fifo, data, sizeof(data));
        printf("enqueue: %s, ", data);

    }
    printf("\n");
    for(int i=5; i<13; i++) {
        char *data = fifo_dequeue(fifo, NULL);
        printf("dequeue: %s, ", data);
        free(data);
    }
    printf("\n");

    test_clean_with_allocator();
}

static void run_tests(void) {
    test_enqueue_and_dequeue();
    test_enqueue_and_dequeue_with_allocator();
}

test_fifo_queue_t test_fifo_queue = { .run_tests = run_tests };