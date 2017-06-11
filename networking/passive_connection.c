//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "passive_connection.h"

result_t listen_connections(sock_fd_t ps_fd) {

    if( listen(ps_fd, BACKLOG) < 0 ) {
        fprintf(stderr, "listen: %s\n", strerror(errno));
        return FAILURE;
    }
    printf("Start listening...\n");

    return SUCCESS;
}

sock_fd_t accept_new_connection(sock_fd_t ps_fd) {

    int cs_fd; // connection socket file descriptor
    struct sockaddr sockaddr;
    socklen_t sockaddrlen = sizeof(sockaddr);

    if( (cs_fd = accept(ps_fd, &sockaddr, &sockaddrlen)) < 0 ) {
        if( (errno == EINTR) || (errno == EAGAIN) || (errno == EWOULDBLOCK)) {
            return CONTINUE;
        }
        fprintf(stderr, "accept: %s\n", strerror(errno));
        return FAILURE;
    }

    print_socket_address(cs_fd, CONNECTION_SOCKET);

    return cs_fd;
}