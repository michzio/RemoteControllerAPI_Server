//
// Created by Michal Ziobro on 10/08/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_TEST_THREADS_MANAGER_H
#define REMOTECONTROLLERAPI_SERVER_TEST_THREADS_MANAGER_H

typedef struct {
    void (*run_tests)(void);
} test_threads_manager_t;

extern test_threads_manager_t test_threads_manager;

#endif //REMOTECONTROLLERAPI_SERVER_TEST_THREADS_MANAGER_H
