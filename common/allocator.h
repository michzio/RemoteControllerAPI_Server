//
// Created by Michal Ziobro on 04/08/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_ALLOCATOR_H
#define REMOTECONTROLLERAPI_SERVER_ALLOCATOR_H

#include "types.h"
#include <ntsid.h>

struct allocator;
typedef struct allocator allocator_t;

typedef void (*allocate_handler_t)(void **data_store, void *data, size_t data_size);
typedef void (*deallocate_handler_t)(void **data_store);

// allocator operations
result_t allocator_init(allocator_t **allocator, allocate_handler_t allocate_handler, deallocate_handler_t deallocate_handler);
allocate_handler_t allocator_allocate(allocator_t *allocator);
deallocate_handler_t allocator_deallocate(allocator_t *allocator);
void allocator_free(allocator_t *allocator);

// allocator handlers
void string_allocate_handler(void **data_store, void *data, size_t data_size);
void string_deallocate_handler(void **data_store);

#endif //REMOTECONTROLLERAPI_SERVER_ALLOCATOR_H
