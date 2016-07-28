//
// Created by Michal Ziobro on 28/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_DOUBLY_LINKED_LIST_H
#define REMOTECONTROLLERAPI_SERVER_DOUBLY_LINKED_LIST_H

struct doubly_linked_node;
typedef struct doubly_linked_node doubly_linked_node_t;

struct doubly_linked_list;
typedef struct doubly_linked_list doubly_linked_list_t;

typedef void (*data_handler_t)(void *);

// list operations
void travers_forward(const doubly_linked_list_t *list, data_handler_t data_handler);
void travers_backward(const doubly_linked_list_t *list, data_handler_t data_handler);
void insert_at_pos(doubly_linked_list_t *list, doubly_linked_node_t *pos, void *data);
void insert_node_at_pos(doubly_linked_list_t *list, doubly_linked_node_t *pos, doubly_linked_node_t *new_node);
void push_front(doubly_linked_list_t *list, void *data);
void push_node_front(doubly_linked_node_t *list, doubly_linked_node_t *new_node);
void push_back(doubly_linked_list_t *list, void *data);
void push_node_back(doubly_linked_node_t *list, doubly_linked_node_t *new_node);
void remove_node(doubly_linked_node_t *list, doubly_linked_node_t *old_node);
void front(const doubly_linked_node_t *list);
void back(const doubly_linked_node_t *list);
void pop_front(doubly_linked_node_t *list);
void pop_back(doubly_linked_node_t *list);

#endif //REMOTECONTROLLERAPI_SERVER_DOUBLY_LINKED_LIST_H
