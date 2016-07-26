//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include <string.h>
#include "service.h"

result_t rpc_service_handler(sock_fd_t sock_fd) {

    // TODO

    return SUCCESS;
}

result_t event_service_handler(sock_fd_t sock_fd) {

    // TODO

    return SUCCESS;
}

#define MAX_BUF_SIZE 256

result_t echo_service_handler(sock_fd_t sock_fd) {

    char buf[MAX_BUF_SIZE];
    int n_recv; // number of bytes received
    int n_sent; // number of bytes sent

    if( (n_recv = recv(sock_fd, buf, sizeof(buf) -1, 0)) <= 0) {
        perror("recv");
        return FAILURE;
    }

    buf[n_recv] = '\0';

    printf("server: received '%s'\n", buf);

    if( (n_sent = send(sock_fd, buf, strlen(buf), 0)) < 0) {
        perror("send");
        return FAILURE;
    }

    return SUCCESS;
}