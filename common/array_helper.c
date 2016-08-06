//
// Created by Michal Ziobro on 31/07/2016.
//
#include <stdio.h>
#include "array_helper.h"

/**
 * function finds index of given element in the array using comparison function - O(n) linear time search
 */
int array_find_int(const int *array, const size_t array_size, const int elem, compare_int_func_t cmp_func) {

    for(int i=0; i<array_size; i++) {
        if(cmp_func(array[i], elem) == 0)
            return i;
    }
    return FAILURE;
}

int array_find(const void **array, const size_t array_size, const void *elem, compare_func_t cmp_func) {

    for(int i=0; i<array_size; i++) {
        if(cmp_func(array[i], elem) == 0)
            return i;
    }
    return FAILURE;
}

/**
 * function removes element with given index from the array and shifts remaining elements to fill the hole made by removing
 */
result_t array_remove_int(int *array, const size_t array_size, const unsigned int elem_idx) {

    if(elem_idx >= array_size || array == 0)
        return FAILURE;

    for(int i = elem_idx; i < (array_size-1); i++)
        array[i] = array[i + 1];

    return SUCCESS;
}

result_t array_remove(void **array, const size_t array_size, const unsigned int elem_idx) {

    if(elem_idx > array_size || array == NULL)
        return FAILURE;

    for(int i = elem_idx; i < (array_size-1); i++)
        array[i] = array[i + 1];

    return SUCCESS;
}

void array_print_int(const int *array, const size_t array_size) {

    for(int i=0; i<array_size; i++)
        printf("%d, ", array[i]);
    printf("\n");
}

void array_print(const void **array, const size_t array_size,  printer_t printer) {
    for (int i = 0; i < array_size; i++) {
        printer(array[i]);
        printf(", ");
    }
    printf("\n");
}

void string_printer(const void *element) {
    printf("%s", (const char *) element);
}

void pointer_printer(const void *element) {
    printf("%p", element);
}