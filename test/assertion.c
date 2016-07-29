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

void assert_equal(const void *l_arg, const void *r_arg, compare_func_t cmp_func, const char *test_name) {
    if(cmp_func(l_arg, r_arg) == 0)
        printf("%s: passed.\n", test_name);
    else
        fprintf(stderr, "%s: failed!\n", test_name);
}