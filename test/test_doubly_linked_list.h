//
// Created by Michal Ziobro on 28/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_TEST_DOUBLY_LINKED_LIST_H
#define REMOTECONTROLLERAPI_SERVER_TEST_DOUBLY_LINKED_LIST_H

#include "../collections/doubly_linked_list.h"

typedef struct {
    void (*run_tests)(void);
} test_doubly_linked_list_t;

extern test_doubly_linked_list_t test_doubly_linked_list;

#endif //REMOTECONTROLLERAPI_SERVER_TEST_DOUBLY_LINKED_LIST_H
