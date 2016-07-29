//
// Created by Michal Ziobro on 29/07/2016.
//

/**
 * FIFO queue (first in, first out) based on
 * the doubly linked list implementation.
 *  |x| in --> | | | | |x|x| -> out |x|
 *  Basic operations:
 *  init(), enqueue(), dequeue(), free()
 */

#ifndef REMOTECONTROLLERAPI_SERVER_FIFO_QUEUE_H
#define REMOTECONTROLLERAPI_SERVER_FIFO_QUEUE_H

struct fifo_queue;
typedef struct fifo_queue fifo_queue_t;

void fifo_init(fifo_queue_t **fifo);
void fifo_enqueue(fifo_queue_t *fifo, void *data, size_t data_size);
void *fifo_dequeue(fifo_queue_t *fifo, size_t *data_size);
void fifo_free(fifo_queue_t *fifo);

#endif //REMOTECONTROLLERAPI_SERVER_FIFO_QUEUE_H
