//
// Created by Michal Ziobro on 31/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_TEST_ARRAY_HELPER_H
#define REMOTECONTROLLERAPI_SERVER_TEST_ARRAY_HELPER_H

typedef struct {
    void (*run_tests)(void);
} test_array_helper_t;

extern test_array_helper_t test_array_helper;

#endif //REMOTECONTROLLERAPI_SERVER_TEST_ARRAY_HELPER_H
