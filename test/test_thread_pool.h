//
// Created by Michal Ziobro on 01/08/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_TEST_THREAD_POOL_H
#define REMOTECONTROLLERAPI_SERVER_TEST_THREAD_POOL_H

typedef struct {
    void (*run_tests)(void);
} test_thread_pool_t;

extern test_thread_pool_t test_thread_pool;

#endif //REMOTECONTROLLERAPI_SERVER_TEST_THREAD_POOL_H
