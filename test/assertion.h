//
// Created by Michal Ziobro on 28/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_ASSERTION_H
#define REMOTECONTROLLERAPI_SERVER_ASSERTION_H

#include "../collections/doubly_linked_list.h"

void assert_not_null(const void *arg, const char *test_name);
void assert_null(const void *arg, const char *test_name);
void assert_equal(const void *l_arg, const void *r_arg, compare_func_t cmp_func, const char *test_name);
void assert_equal_int(const int l_arg, const int r_arg, const char *test_name);
void assert_greater_than(const int l_arg, const int r_arg, const char *test_name);

#endif //REMOTECONTROLLERAPI_SERVER_ASSERTION_H
