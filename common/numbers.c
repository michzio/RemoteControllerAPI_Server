//
// Created by Michal Ziobro on 04/08/2016.
//

#include <stdlib.h>
#include <stdio.h>

int max(const int a, const int b) {
    return (a > b) ? a : b;
}
int min(const int a, const int b) {
    return (a < b) ? a : b;
}

int range_rand(int min_num, int max_num) {
    if(min_num >= max_num) {
        fprintf(stderr, "min_num is greater or equal than max_num!\n");
    }
    return min_num + (rand() % (max_num - min_num));
}