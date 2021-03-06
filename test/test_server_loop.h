//
// Created by Michal Ziobro on 13/08/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_TEST_SERVER_LOOPS_H
#define REMOTECONTROLLERAPI_SERVER_TEST_SERVER_LOOPS_H

typedef struct {
    void (*run_tests)(void);
} test_server_loop_t;

extern test_server_loop_t test_server_loop;

#define TEST_PORT "3333"
#define LOCALHOST "192.168.8.101"

#endif //REMOTECONTROLLERAPI_SERVER_TEST_SERVER_LOOPS_H
