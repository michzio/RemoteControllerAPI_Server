//
// Created by Michal Ziobro on 29/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_TEST_FIFO_QUEUE_H
#define REMOTECONTROLLERAPI_SERVER_TEST_FIFO_QUEUE_H

typedef struct {
    void (*run_tests)(void);
} test_fifo_queue_t;

extern test_fifo_queue_t test_fifo_queue;

#endif //REMOTECONTROLLERAPI_SERVER_TEST_FIFO_QUEUE_H
