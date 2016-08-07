//
// Created by Michal Ziobro on 28/07/2016.
//

#include <stdio.h>
#include "assertion.h"

void assert_not_null(const void *arg, const char *test_name) {
    if(arg != 0)
        printf("%s: passed.\n", test_name);
    else
        fprintf(stderr, "%s: failed!\n", test_name);
}

void assert_null(const void *arg, const char *test_name) {
    if(arg == 0)
        printf("%s: passed.\n", test_name);
    else
        fprintf(stderr, "%s: failed!\n", test_name);
}

void assert_equal(const void *l_arg, const void *r_arg, compare_func_t cmp_func, const char *test_name) {
    if(cmp_func(l_arg, r_arg) == 0)
        printf("%s: passed.\n", test_name);
    else
        fprintf(stderr, "%s: failed!\n", test_name);
}

void assert_equal_int(const int l_arg, const int r_arg, const char *test_name) {
    if(int_cmp_func(l_arg, r_arg) == 0)
        printf("%s: passed.\n", test_name);
    else
        fprintf(stderr, "%s: failed! (%d <> %d)\n", test_name, l_arg, r_arg);
}

void assert_greater_than(const int l_arg, const int r_arg, const char *test_name) {
    if(int_cmp_func(l_arg, r_arg) > 0)
        printf("%s: passed.\n", test_name);
    else
        fprintf(stderr, "%s: failed! !(%d > %d)\n", test_name, l_arg, r_arg);
}

void assert_less_than(const int l_arg, const int r_arg, const char *test_name) {
    if(int_cmp_func(l_arg, r_arg) < 0)
        printf("%s: passed.\n", test_name);
    else
        fprintf(stderr, "%s: failed! !(%d < %d)\n", test_name, l_arg, r_arg);
}

void assert_in_range(const int arg, const int min, const int max, const char *test_name) {
    if( int_cmp_func(arg, min) >= 0 && int_cmp_func(arg, max) <= 0 )
        printf("%s: passed.\n", test_name);
    else
        fprintf(stderr, "%s: failed! %d not in [%d,%d]\n", test_name, arg, min, max);
}