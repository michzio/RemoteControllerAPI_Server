//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include <string.h>
#include "service.h"

result_t rpc_service_connection_handler(sock_fd_t sock_fd) {

    // TODO

    return SUCCESS;
}

result_t event_service_connection_handler(sock_fd_t sock_fd) {

    // TODO

    return SUCCESS;
}

#define MAX_BUF_SIZE 256

result_t echo_service_connection_handler(sock_fd_t sock_fd) {

    char buf[MAX_BUF_SIZE];
    int n_recv; // number of bytes received
    int n_sent; // number of bytes sent

    while(1) {

        if ((n_recv = recv(sock_fd, buf, sizeof(buf) - 1, 0)) <= 0) {
            perror("recv");
            return FAILURE;
        }

        buf[n_recv] = '\0';

        printf("server: received '%s'\n", buf);

        if ((n_sent = send(sock_fd, buf, strlen(buf), 0)) < 0) {
            perror("send");
            return FAILURE;
        }
    }

    return SUCCESS;
}

result_t echo_service_datagram_handler(sock_fd_t sock_fd, const struct sockaddr *peer_addr, const char *datagram) {

    int n_sent; // number of bytes sent

    printf("server: received '%s'\n", datagram);

    if ((n_sent = sendto(sock_fd, datagram, strlen(datagram), 0, peer_addr, sizeof(*peer_addr))) < 0) {
        perror("send");
        return FAILURE;
    }

    return SUCCESS;
}