//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "server_loop.h"

// TCP

/**
 * Function is looping infinitely and waiting
 * for new incoming client connections.
 * It handles connections one by one.
 */
result_t iterative_stream_server_loop(sock_fd_t ps_fd, connection_handler_t handle_connection) {

    int cs_fd;

    printf("Waiting for new connections on the main thread...\n");

    while(1) {
        cs_fd = accept_new_connection(ps_fd);

        if(cs_fd == FAILURE) {
            fprintf(stderr, "accept_new_connection: failed!\n");
            return FAILURE;
        } else if(cs_fd == CONTINUE) {
            continue;
        }

        printf("Handle connection on the main thread...");

        if(handle_connection(cs_fd) < 0) {
            fprintf(stderr, "handle_connection: failed!\n");
        }

        if(close(cs_fd) < 0){
            fprintf(stderr, "close: %s\n", strerror(errno));
            return FAILURE;
        }
    }
}

result_t concurrent_stream_server_loop(sock_fd_t ps_fd, connection_handler_t conn_handler) {

    // TODO

    return SUCCESS;
}

result_t pseudo_concurrent_stream_server_loop(sock_fd_t ps_fd, connection_handler_t conn_handler) {

    // TODO

    return SUCCESS;
}

#define MAX_BUF_SIZE 256

// UDP
result_t iterative_datagram_server_loop(sock_fd_t ps_fd, datagram_handler_t handle_datagram) {

    char buf[MAX_BUF_SIZE];
    int n_recv; // number of bytes received
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);

    printf("Waiting for new datagrams on the main thread...\n");

    while(1) {
        if( (n_recv = recvfrom(ps_fd, buf, sizeof(buf)-1, 0, (struct sockaddr *) &peer_addr, &peer_addr_len)) < 0 ) {
            fprintf(stderr, "recvfrom: %s\n", strerror(errno));
            return FAILURE;
        }
        buf[n_recv] = '\0';

        printf("Handle datagram on the main thread...");

        if(handle_datagram(ps_fd, (struct sockaddr *) &peer_addr, buf) == FAILURE) {
            fprintf(stderr, "handle_datagram: %s\n", strerror(errno));
            continue;
        }
    }
}

result_t concurrent_datagram_server_loop(sock_fd_t ps_fd, datagram_handler_t datagram_handler) {

    // TODO

    return SUCCESS;
}