//
// Created by Michal Ziobro on 29/07/2016.
//

#include <stdlib.h>
#include "../collections/doubly_linked_list.h"
#include "fifo_queue.h"
#include <stdio.h>
#include <string.h>

struct fifo_queue {
    doubly_linked_list_t *queue;
};

/**
 * function allocates memory for fifo queue
 * and its internal doubly linked list used
 * as the queue data structure.
 */
void fifo_init(fifo_queue_t **fifo) {

    *fifo = malloc(sizeof(fifo_queue_t));
    list_init( &((*fifo)->queue) );
}

void fifo_enqueue(fifo_queue_t *fifo, void *data, size_t data_size) {
    push_front(fifo->queue, data, data_size);
}

void *fifo_dequeue(fifo_queue_t *fifo, size_t *data_size) {

    doubly_linked_node_t *node;
    size_t tmp_size;
    void *tmp_data;
    // get data and its size from the last node in the queue
    if((node = back(fifo->queue)) == NULL) {
        fprintf(stderr, "back: queue is empty!\n");
        data_size = NULL;
        return NULL;
    };
    tmp_data = unwrap_data(node, &tmp_size);

    // copy retrieved data
    void *data = malloc(tmp_size);
    memcpy(data, tmp_data, tmp_size);

    // remove the last node in the queue
    pop_back(fifo->queue);

    if(data_size != NULL) *data_size = tmp_size; // return size of data through pointer argument
    return data;
}

void fifo_free(fifo_queue_t *fifo) {

    free_doubly_linked_list(fifo->queue);
    free(fifo);
    fifo = NULL;
}