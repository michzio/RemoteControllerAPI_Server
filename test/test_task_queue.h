//
// Created by Michal Ziobro on 30/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_TEST_TASK_QUEUE_H
#define REMOTECONTROLLERAPI_SERVER_TEST_TASK_QUEUE_H

typedef struct {
    void (*run_tests)(void);
} test_task_queue_t;

extern test_task_queue_t test_task_queue;

#endif //REMOTECONTROLLERAPI_SERVER_TEST_TASK_QUEUE_H
