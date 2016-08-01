//
// Created by Michal Ziobro on 31/07/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include "test_array_helper.h"
#include "../common/array_helper.h"
#include "assertion.h"

#define ARRAY_SIZE 10
#define STR_LEN 256

static char **str_array;
static int *int_array;

static void test_create(void) {

    str_array = malloc(sizeof(char *) * ARRAY_SIZE);

    for(int i=0; i<ARRAY_SIZE; i++) {
        char *str = malloc(STR_LEN);
        sprintf(str, "test %d", i+1);
        str_array[i] = str;
    }

    int_array = malloc(sizeof(int) * ARRAY_SIZE);

    for (int i=0; i<ARRAY_SIZE; i++)
        int_array[i] = i+1;
}

static void test_clean(void) {

    free(str_array);
    free(int_array);
}

static void test_array_find_int(void) {
    test_create();
    printf("%s:\n", __func__);
    array_print_int(int_array, ARRAY_SIZE);
    int idx_of_5 = array_find_int(int_array, ARRAY_SIZE, 5, int_cmp_func);
    int idx_of_7 = array_find_int(int_array, ARRAY_SIZE, 7, int_cmp_func);
    assert_equal_int(idx_of_5, 4, "find idx of 5");
    assert_equal_int(idx_of_7, 6, "find idx of 7");
    test_clean();
}

static void test_array_find(void) {
    test_create();
    printf("%s:\n", __func__);
    array_print((const void **)str_array, ARRAY_SIZE, string_printer);
    int idx_of_test_5 = array_find((const void **)str_array, ARRAY_SIZE, "test 5", str_cmp_func);
    int idx_of_test_7 = array_find((const void **)str_array, ARRAY_SIZE, "test 7", str_cmp_func);
    assert_equal_int(idx_of_test_5, 4, "find idx of 'test 5'");
    assert_equal_int(idx_of_test_7, 6, "find idx of 'test 7'");
    test_clean();
}

static void test_array_remove_int(void) {
    test_create();
    printf("%s:\n", __func__);
    array_print_int(int_array, ARRAY_SIZE);
    int idx_of_5 = array_find_int(int_array, ARRAY_SIZE, 5, int_cmp_func);
    assert_equal_int(idx_of_5, 4, "find int 5 in array");
    array_remove_int(int_array, ARRAY_SIZE, idx_of_5);
    array_print_int(int_array, ARRAY_SIZE);
    idx_of_5 = array_find_int(int_array, ARRAY_SIZE, 5, int_cmp_func);
    assert_equal_int(idx_of_5, FAILURE, "not find int 5 in array");
    test_clean();
}

static void test_array_remove(void) {
    test_create();
    printf("%s:\n", __func__);
    array_print((const void **)str_array, ARRAY_SIZE, string_printer);
    int idx_of_test_5 = array_find((const void **)str_array, ARRAY_SIZE, "test 5", str_cmp_func);
    assert_equal_int(idx_of_test_5, 4, "find 'test 5' in array");
    array_remove((void **)str_array, ARRAY_SIZE, idx_of_test_5);
    array_print((const void **)str_array, ARRAY_SIZE, string_printer);
    idx_of_test_5 = array_find((const void **)str_array, ARRAY_SIZE, "test 5", str_cmp_func);
    assert_equal_int(idx_of_test_5, FAILURE, "not find 'test 5' in array");
    test_clean();
}

static void run_tests(void) {
    test_array_find_int();
    test_array_find();
    test_array_remove_int();
    test_array_remove();
}

test_array_helper_t test_array_helper = { .run_tests = run_tests };