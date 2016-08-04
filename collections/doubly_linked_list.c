//
// Created by Michal Ziobro on 28/07/2016.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "doubly_linked_list.h"
#include "../common/types.h"

struct doubly_linked_node {
    doubly_linked_node_t *prev; // a reference to the previous node
    doubly_linked_node_t *next; // a reference to the next node
    void *data;                 // reference to any data stored in the node    (should store only heap allocated data, else free(node->data) fails).
    size_t data_size;           // size of data stored in the node
};

struct doubly_linked_list {
    doubly_linked_node_t *head;     // points to head (first) node of the list
    doubly_linked_node_t *tail;     // points to tail (last) node of the list
    allocator_t *allocator;         // allocator can be null, or set as allocator_t struct that enables to allocate/deallocate nodes
};

/**
 * while initializing a list you can pass optional allocator.
 * @allocator is the struct containing allocate_handler and
 * deallocate_handler.
 * if allocator is null, nodes store only pointers to data
 * owned by clients of the list.
 * while allocator is defined, nodes allocate/deallocate
 * data as owned by the list.
 */
void list_init(doubly_linked_list_t **list, allocator_t *allocator) {
    *list = malloc(sizeof(doubly_linked_list_t));
    (*list)->head = NULL;
    (*list)->tail = NULL;
    (*list)->allocator = allocator;
}

void travers_forward(const doubly_linked_list_t *list, data_handler_t handle_data) {

    doubly_linked_node_t *node = list->head;
    if(node == NULL) {
        fprintf(stderr, "Doubly linked list is empty!");
    }
    do {
        handle_data(node->data, node->data_size);
        node = node->next;
    } while (node != list->head);
}

void travers_backward(const doubly_linked_list_t *list, data_handler_t handle_data) {

    doubly_linked_node_t *node = list->tail;
    if(node == NULL) {
        fprintf(stderr, "Doubly linked list is empty!");
    }
    do {
        handle_data(node->data, node->data_size);
        node = node->prev;
    } while (node != list->tail);
}

doubly_linked_node_t *find_first(const doubly_linked_list_t *list, const void *data, compare_func_t cmp_func) {

    doubly_linked_node_t *node = list->head;
    if(node == NULL) {
        fprintf(stderr, "Doubly linked list is empty!");
        return NULL;
    }
    do {
        if(cmp_func(data, node->data) == 0)
            return node;
        node = node->next;
    } while (node != list->head);

    return NULL;
}

void insert_at_pos(doubly_linked_list_t *list, doubly_linked_node_t *pos, void *data, size_t data_size) {

    // wrap data into node
    doubly_linked_node_t *new_node;
    node_init(&new_node);
    wrap_data(list, new_node, data, data_size);

    insert_node_at_pos(list, pos, new_node);
}

void insert_node_at_pos(doubly_linked_list_t *list, doubly_linked_node_t *pos, doubly_linked_node_t *new_node) {

    if(pos == NULL) {
        push_node_front(list, new_node);
        return;
    }

    new_node->prev = pos->prev;
    new_node->next = pos;
    if(pos == list->head)
        list->head = new_node;
    pos->prev->next = new_node;
    pos->prev = new_node;

}

void push_front(doubly_linked_list_t *list, void *data, size_t data_size) {

    // wrap data into node
    doubly_linked_node_t *new_node;
    node_init(&new_node);
    wrap_data(list, new_node, data, data_size);

    push_node_front(list, new_node);
}

void push_node_front(doubly_linked_list_t *list, doubly_linked_node_t *new_node) {

    // if empty list than place first node on it
    if(list->head == NULL) {
        new_node->next = new_node;
        new_node->prev = new_node;
        list->head = new_node;
        list->tail = new_node;
        return;
    }

    // else place new node at the beginning of the list
    new_node->next = list->head;
    new_node->prev = list->head->prev;
    list->head->prev->next = new_node;
    list->head->prev = new_node;

    list->head = new_node;
}

void push_back(doubly_linked_list_t *list, void *data, size_t data_size) {

    // wrap data into node
    doubly_linked_node_t *new_node;
    node_init(&new_node);
    wrap_data(list, new_node, data, data_size);

    push_node_back(list, new_node);
}

void push_node_back(doubly_linked_list_t *list, doubly_linked_node_t *new_node) {

    // if empty list than place first node on it
    if(list->tail == NULL) {
        new_node->next = new_node;
        new_node->prev = new_node;
        list->head = new_node;
        list->tail = new_node;
        return;
    }

    // else place new node at the end of the list
    new_node->next = list->tail->next;
    new_node->prev = list->tail;
    list->tail->next->prev = new_node;
    list->tail->next = new_node;

    list->tail = new_node;
}

void remove_node(doubly_linked_list_t *list, doubly_linked_node_t *old_node) {

    // if node to delete unspecified return error
    if (old_node == NULL) {
        fprintf(stderr, "Node to delete is empty!");
        return;
    }

    // if node to delete is the only left on the list make empty list
    if (old_node->next == old_node) // or old_node->prev = old_node
    {
        list->head = list->tail = NULL;
    } else {
        old_node->prev->next = old_node->next;
        old_node->next->prev = old_node->prev;

        if(old_node == list->head) list->head = old_node->next;
        if(old_node == list->tail) list->tail = old_node->prev;
    }

    node_free(list, old_node);
}

doubly_linked_node_t *front(const doubly_linked_list_t *list) {
    return list->head;
}

doubly_linked_node_t *back(const doubly_linked_list_t *list) {
    return list->tail;
}

void pop_front(doubly_linked_list_t *list) {
    remove_node(list, list->head);
}

void pop_back(doubly_linked_list_t *list) {
    remove_node(list, list->tail);
}

void list_free(doubly_linked_list_t *list) {

    doubly_linked_node_t *node = list->head;

    // deallocate nodes
    while(node) {
        doubly_linked_node_t *del_node = node;
        node = node->next;
        node_free(list, del_node);
        // if reached pointer tail -> head, just break
        if(node == list->head) break;
    }
    list->head = NULL; list->tail = NULL;

    // deallocate allocator if set
    if(list->allocator != NULL) allocator_free(list->allocator);

    // deallocate list
    free(list);
    list = NULL;
}

void node_init(doubly_linked_node_t **node) {
    *node = malloc(sizeof(doubly_linked_node_t));
    (*node)->prev = NULL;
    (*node)->next = NULL;
    (*node)->data = NULL;
    (*node)->data_size = 0;
}

void *unwrap_data(doubly_linked_node_t *node, size_t *data_size) {
    if(data_size != NULL) *data_size = node->data_size; // return size of data through pointer argument
    return node->data;
}

/**
 * function wrap data into list node. It takes data and its size.
 * if there is allocator defined and list is the owner of data
 * stored in the node we should properly allocate this data
 * with allocator. on the other hand if client is the owner of data
 * we just store pointer to the data and leaves to the client
 * proper allocation/deallocation of this data.
 * regardless of data ownership store its size in the node.
 */
void wrap_data(doubly_linked_list_t *list, doubly_linked_node_t *node, void *data, size_t data_size) {

    // store data size in the node
    node->data_size = data_size;

    if(list->allocator != NULL) {
        // when list is owning data allocate it with allocator
        allocate_handler_t allocate = allocator_allocate(list->allocator);
        allocate(&node->data, data, data_size);
    } else {
        // when client is owning data only store pointer to it
        node->data = data;
    }
}

/**
 * function deallocates given list node. if there is allocator
 * defined and list is the owner of data stored in the node
 * we should properly deallocate this data with allocator struct.
 * on the other hand if client is the owner of data we skip this
 * step, as the client is the only responsible for dealloaction
 * of this data. regardless of data ownership we always free node struct.
 */
void node_free(doubly_linked_list_t *list, doubly_linked_node_t *node) {

    // when list is owning data free it
    if(list->allocator != NULL) {
        deallocate_handler_t deallocate = allocator_deallocate(list->allocator);
        deallocate(&node->data);
    }
    // when client is owning data it is responsible for its deallocation

    // regardless of data ownership free node
    free(node);
    node = NULL;
}