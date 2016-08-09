//
// Created by Michal Ziobro on 27/07/2016.
//

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "thread.h"

// create thread: generic thread, connection thread, datagram thread
pthread_t thread_create(runner_t runner, runner_attr_t runner_attr) {

    pthread_t thread;
    pthread_attr_t thread_attr;

    // fill pthread attributes
    pthread_attr_init(&thread_attr);

    if( pthread_create(&thread, &thread_attr, runner, runner_attr) != 0 )  {
        fprintf(stderr, "pthread_create: failed to create thread.\n");
        return NULL;
    }

    return thread;
}

/**
 * @conn_handler - implements protocol used during communication between client and server
 * @conn_sock_fd - socket file descriptor used to communicate with connected client
 * @callback - optional callback function executed at the end of communication with results of conn_handler
 */
pthread_t connection_thread(connection_handler_t conn_handler, sock_fd_t conn_sock_fd, callback_t callback) {

    conn_thread_runner_attr_t *conn_thread_runner_attr;
    conn_thread_runner_attr_init(&conn_thread_runner_attr);
    conn_thread_runner_attr_fill(conn_thread_runner_attr, conn_handler, conn_sock_fd, callback);

    return thread_create((runner_t) connection_thread_runner, (runner_attr_t) conn_thread_runner_attr);
}
/**
 * @datagram_handler - implements protocol used to handle datagram request from client and send him datagram response
 * @sock_fd - socket file descriptor used to send response back to the client
 * @peer_addr - peer address, where to send response back
 * @datagram - request datagram
 * @callback - optional callback function executed after datagram handling with results of datagram_handler
 */
pthread_t datagram_thread(datagram_handler_t datagram_handler, sock_fd_t sock_fd, struct sockaddr *peer_addr, char *datagram, callback_t callback) {

    datagram_thread_runner_attr_t *datagram_thread_runner_attr;
    datagram_thread_runner_attr_init(&datagram_thread_runner_attr);
    datagram_thread_runner_attr_fill(datagram_thread_runner_attr, datagram_handler, sock_fd, peer_addr, datagram, callback);

    return thread_create((runner_t) datagram_thread_runner, (runner_attr_t) datagram_thread_runner_attr);
}




