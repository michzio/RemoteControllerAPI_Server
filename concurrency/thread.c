//
// Created by Michal Ziobro on 27/07/2016.
//

#include <pthread.h>
#include <stdio.h>
#include "thread.h"

result_t connection_thread(sock_fd_t conn_sock_fd, connection_handler_t conn_handler) {

    pthread_t conn_thread_id;

    if( pthread_create(&conn_thread_id, NULL /* attr */, conn_handler, &conn_sock_fd) != 0 )  {
        fprintf(stderr, "pthread_create: failed to create connection thread.\n");
        return FAILURE;
    }

    return conn_thread_id;
}