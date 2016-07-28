//
// Created by Michal Ziobro on 28/07/2016.
//

#include "doubly_linked_list.h"

struct doubly_linked_node {
    doubly_linked_node_t *prev; // a reference to the previous node
    doubly_linked_node_t *next; // a reference to the next node
    void *data; // reference to any data stored in the node
};

struct doubly_linked_list {
    doubly_linked_node_t *head; // points to head (first) node of the list
    doubly_linked_node_t *tail;  // points to tail (last) node of the list
};

void travers_forward(const doubly_linked_list_t *list, data_handler_t data_handler) {
    // TODO
}

void travers_backward(const doubly_linked_list_t *list, data_handler_t data_handler) {
    // TODO
}

void insert_at_pos(doubly_linked_list_t *list, doubly_linked_node_t *pos, void *data) {
    // TODO
}

void insert_node_at_pos(doubly_linked_list_t *list, doubly_linked_node_t *pos, doubly_linked_node_t *new_node) {
    // TODO
}

void push_front(doubly_linked_list_t *list, void *data) {
    // TODO
}

void push_node_front(doubly_linked_node_t *list, doubly_linked_node_t *new_node) {
    // TODO
}

void push_back(doubly_linked_list_t *list, void *data) {
    // TODO
}

void push_node_back(doubly_linked_node_t *list, doubly_linked_node_t *new_node) {
    // TODO
}

void remove_node(doubly_linked_node_t *list, doubly_linked_node_t *old_node) {
    // TODO
}

void front(const doubly_linked_node_t *list) {
    // TODO
}

void back(const doubly_linked_node_t *list) {
    // TODO
}

void pop_front(doubly_linked_node_t *list) {
    // TODO
}

void pop_back(doubly_linked_node_t *list) {
    // TODO
}
