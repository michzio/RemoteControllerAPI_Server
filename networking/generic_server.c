//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "generic_server.h"

result_t create_stream_server(const char *port, stream_server_loop_t server_loop, connection_handler_t conn_handler) {

    int ps_fd; // passive socket file descriptor

    printf("Creating new stream server...\n");

    if(create_stream_pasv_sock(port, &ps_fd) == FAILURE) {
        fprintf(stderr, "create_stream_pasv_sock: failed!\n");
        return FAILURE;
    }

    if(listen_connections(ps_fd) == FAILURE) {
        fprintf(stderr, "listen_connections: failed!\n");
        return FAILURE;
    }

    if(server_loop(ps_fd, conn_handler) == FAILURE) {
        fprintf(stderr, "server_loop: failed!\n");
        return FAILURE;
    }

    if(close(ps_fd) < 0) {
        fprintf(stderr, "close: %s\n", strerror(errno));
        return FAILURE;
    }

    return SUCCESS;
}

result_t create_datagram_server(const char *port, datagram_server_loop_t server_loop, datagram_handler_t datagram_handler) {

    int ps_fd; // passive socket file descriptor

    printf("Creating new datagram server...\n");

    if(create_datagram_pasv_sock(port, &ps_fd) == FAILURE) {
        fprintf(stderr, "create_datagram_pasv_sock: failed!\n");
        return FAILURE;
    }

    if(server_loop(ps_fd, datagram_handler) == FAILURE) {
        fprintf(stderr, "server_loop: failed!\n");
        return FAILURE;
    }

    if(close(ps_fd) < 0) {
        fprintf(stderr, "close: %s\n", strerror(errno));
        return FAILURE;
    }

    return SUCCESS;
}