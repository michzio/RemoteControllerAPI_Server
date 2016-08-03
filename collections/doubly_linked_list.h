//
// Created by Michal Ziobro on 28/07/2016.
//

/**
 * Doubly linked list - collection structure based
 * on nodes linked in both directions using pointers.
 * This linked list is circular i.e. last node points
 * to first and first to last.
 *   <- |n| <-> |n| <-> |n| ->....<-|n|->
 */

#ifndef REMOTECONTROLLERAPI_SERVER_DOUBLY_LINKED_LIST_H
#define REMOTECONTROLLERAPI_SERVER_DOUBLY_LINKED_LIST_H

#include "../common/comparer.h"
#include "../common/types.h"

struct doubly_linked_node;
typedef struct doubly_linked_node doubly_linked_node_t;

struct doubly_linked_list;
typedef struct doubly_linked_list doubly_linked_list_t;

typedef void (*data_handler_t)(void *);

struct allocator;
typedef struct allocator allocator_t;

typedef void (*allocate_handler_t)(doubly_linked_node_t *node, void *data, size_t data_size);
typedef void (*deallocate_handler_t)(doubly_linked_node_t *node);

// list operations
void list_init(doubly_linked_list_t **list, allocator_t *allocator);
void travers_forward(const doubly_linked_list_t *list, data_handler_t data_handler);
void travers_backward(const doubly_linked_list_t *list, data_handler_t data_handler);
doubly_linked_node_t *find_first(const doubly_linked_list_t *list, const void *data, compare_func_t cmp_func);
void insert_at_pos(doubly_linked_list_t *list, doubly_linked_node_t *pos, void *data, size_t data_size);
void insert_node_at_pos(doubly_linked_list_t *list, doubly_linked_node_t *pos, doubly_linked_node_t *new_node);
void push_front(doubly_linked_list_t *list, void *data, size_t data_size);
void push_node_front(doubly_linked_list_t *list, doubly_linked_node_t *new_node);
void push_back(doubly_linked_list_t *list, void *data, size_t data_size);
void push_node_back(doubly_linked_list_t *list, doubly_linked_node_t *new_node);
void remove_node(doubly_linked_list_t *list, doubly_linked_node_t *old_node);
doubly_linked_node_t *front(const doubly_linked_list_t *list);
doubly_linked_node_t *back(const doubly_linked_list_t *list);
void pop_front(doubly_linked_list_t *list);
void pop_back(doubly_linked_list_t *list);
void list_free(doubly_linked_list_t *list);

// node operations
void node_init(doubly_linked_node_t **node);
void *unwrap_data(doubly_linked_node_t *node, size_t *data_size);
void wrap_data(doubly_linked_list_t *list, doubly_linked_node_t *node, void *data, size_t data_size);
void node_free(doubly_linked_list_t *list, doubly_linked_node_t *node);

// allocator operations
result_t allocator_init(allocator_t **allocator, allocate_handler_t allocate_handler, deallocate_handler_t deallocate_handler);
allocate_handler_t allocator_allocate(allocator_t *allocator);
deallocate_handler_t allocator_deallocate(allocator_t *allocator);
void allocator_free(allocator_t *allocator);

// handlers
// data handlers
void print_string_data_handler(void *);

// allocator handlers
void string_allocate_handler(doubly_linked_node_t *node, void *data, size_t data_size);
void string_deallocate_handler(doubly_linked_node_t *node);

#endif //REMOTECONTROLLERAPI_SERVER_DOUBLY_LINKED_LIST_H
