//
// Created by Michal Ziobro on 04/08/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include "data_handler.h"


// data handlers
void print_string_data_handler(void *data, size_t data_size) {

    printf("%s, ", data);
}

void free_data_handler(void *data, size_t data_size) {

    free( (char *) data);
}