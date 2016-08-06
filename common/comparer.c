//
// Created by Michal Ziobro on 31/07/2016.
//

#include <string.h>
#include <pthread.h>

int int_cmp_func(const int a, const int b) {
    return (a < b) ? -1 : (a > b);
}

int str_cmp_func(const void *str1, const void *str2) {
    return strcmp((const char *) str1, (const char *) str2);
}

int pthread_cmp_func(const void *pthread1, const void *pthread2) {

    pthread_t p1 = (pthread_t) pthread1;
    pthread_t p2 = (pthread_t) pthread2;
    // pthread_equal() returns 0 if thread IDs don't match, and non-zero value if they match
    return !pthread_equal(p1, p2);
}