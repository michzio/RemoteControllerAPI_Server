//
// Created by Michal Ziobro on 31/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_COMPARER_H
#define REMOTECONTROLLERAPI_SERVER_COMPARER_H

typedef int (*compare_int_func_t)(const int, const int);
typedef int (*compare_func_t)(const void *, const void *);

int int_cmp_func(const int a, const int b);
int str_cmp_func(const void *str1, const void *str2);
int pthread_cmp_func(const void *pthread1, const void *pthread2);

#endif //REMOTECONTROLLERAPI_SERVER_COMPARER_H
