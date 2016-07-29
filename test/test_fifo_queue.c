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

    fifo_init(&fifo);
}

static void test_clean(void) {
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
        printf("dequeue: %s, ", (char *) fifo_dequeue(fifo, NULL));
    }
    printf("\n");
    for(int i=10; i<13; i++) {
        char *data = malloc(256);
        sprintf(data, "data %d", i);
        fifo_enqueue(fifo, data, sizeof(data));
        printf("enqueue: %s, ", data);

    }
    printf("\n");
    for(int i=0; i<9; i++) {
        printf("dequeue: %s, ", (char *) fifo_dequeue(fifo, NULL));
    }
    printf("\n");
    test_clean();
}

static void run_tests(void) {
    test_enqueue_and_dequeue();
}

test_fifo_queue_t test_fifo_queue = { .run_tests = run_tests };