//
// Created by Michal Ziobro on 31/07/2016.
//

#include <string.h>

int int_cmp_func(const int a, const int b) {
    return (a < b) ? -1 : (a > b);
}

int str_cmp_func(const void *str1, const void *str2) {
    return strcmp((const char *) str1, (const char *) str2);
}

