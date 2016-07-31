//
// Created by Michal Ziobro on 31/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_ARRAY_HELPER_H
#define REMOTECONTROLLERAPI_SERVER_ARRAY_HELPER_H

#include <ntsid.h>
#include "types.h"

typedef void *elem_t;
typedef int (*compare_int_func_t)(const int, const int);
typedef int (*compare_func_t)(const void *, const void *);

int array_find_int(const int *array, const size_t array_size, const int elem, compare_int_func_t cmp_func);
result_t array_remove_int(int *array, const size_t array_size, const unsigned int elem_idx);
int array_find(const elem_t *array, const size_t array_size, const elem_t elem, compare_func_t cmp_func);
result_t array_remove(elem_t *array, const size_t array_size, const unsigned int elem_idx);

#endif //REMOTECONTROLLERAPI_SERVER_ARRAY_HELPER_H
