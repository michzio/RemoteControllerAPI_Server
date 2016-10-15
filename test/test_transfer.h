//
// Created by Michal Ziobro on 15/10/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_TEST_TRANSFER_H
#define REMOTECONTROLLERAPI_SERVER_TEST_TRANSFER_H

typedef struct {
    void (*run_tests)(void);
} test_server_transfer_t;

extern test_server_transfer_t test_server_transfer;

#endif //REMOTECONTROLLERAPI_SERVER_TEST_TRANSFER_H
