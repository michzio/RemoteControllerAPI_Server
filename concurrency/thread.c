//
// Created by Michal Ziobro on 27/07/2016.
//

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "thread.h"

/**
 * wrapping all arguments needed
 * by connection thread runner
 * to handle connection
 * into single structure to make
 * them passable to thread's start routine
 */
struct conn_thread_runner_attr {
    connection_handler_t conn_handler;
    sock_fd_t conn_sock_fd;
};

/**
 * runner function used to wrap invocation of connection handler
 * with its params in order to make it passable as thread's start routine
 */
void *connection_thread_runner(conn_thread_runner_attr_t *attr) {
    attr->conn_handler(attr->conn_sock_fd);
}

result_t connection_thread(connection_handler_t conn_handler, sock_fd_t conn_sock_fd) {

    pthread_t conn_thread_id;
    pthread_attr_t pthread_attr;
    conn_thread_runner_attr_t conn_thread_runner_attr;

    // fill pthread attributes
    pthread_attr_init(&pthread_attr);

    // fill runner attributes
    memset(&conn_thread_runner_attr, 0, sizeof(conn_thread_runner_attr));
    conn_thread_runner_attr.conn_handler = conn_handler;
    conn_thread_runner_attr.conn_sock_fd = conn_sock_fd;

    if( pthread_create(&conn_thread_id, &pthread_attr, connection_thread_runner, &conn_thread_runner_attr) != 0 )  {
        fprintf(stderr, "pthread_create: failed to create connection thread.\n");
        return FAILURE;
    }

    return conn_thread_id;
}




