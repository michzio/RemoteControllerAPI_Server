//
// Created by Michal Ziobro on 31/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_ARRAY_HELPER_H
#define REMOTECONTROLLERAPI_SERVER_ARRAY_HELPER_H

#include <ntsid.h>
#include "types.h"
#include "comparer.h"

typedef void (*printer_t)(const void *);

int array_find_int(const int *array, const size_t array_size, const int elem, compare_int_func_t cmp_func);
result_t array_remove_int(int *array, const size_t array_size, const unsigned int elem_idx);
int array_find(const void **array, const size_t array_size, const void *elem, compare_func_t cmp_func);
result_t array_remove(void **array, const size_t array_size, const unsigned int elem_idx);
void array_print_int(const int *array, const size_t array_size);
void array_print(const void **array, const size_t array_size, printer_t printer);

void string_printer(const void *element);
void pointer_printer(const void *element);

#endif //REMOTECONTROLLERAPI_SERVER_ARRAY_HELPER_H
