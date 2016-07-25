/*
 * Created by Michal Ziobro on 21/07/2016.
 */

#define _REENTRANT
#include <pthread.h> // biblioteka wątków standardu POSIX

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "server.h"


int start_server(void) {

    int ps_fd; // passive socket file descriptor

    printf("Starting new server...\n");

    if(create_passive_socket(&ps_fd) == FAILURE) {
        fprintf(stderr, "create_passive_socket: failed!\n");
        return FAILURE;
    }

    if(listen_connections(ps_fd) == FAILURE) {
        fprintf(stderr, "listen_connections: failed!\n");
        return FAILURE;
    }

    wait_for_connections(ps_fd);

    close(ps_fd);

    return SUCCESS;
}

result_t create_passive_socket(int *res_fd) {

    int ps_fd; // passive socket file descriptor
    struct addrinfo addrinfo_hints, *addrinfo_res, *ai_ptr; // address info structures holding host address information
    int gai_res; // getaddrinfo() result value
    int optval = 1; // used in setsockopt to set option value

    // populating address info hints for getaddrinfo()
    memset(&addrinfo_hints, 0, sizeof(addrinfo_hints));
    addrinfo_hints.ai_flags = AI_PASSIVE; // current host IP
    addrinfo_hints.ai_family = AF_UNSPEC;
    addrinfo_hints.ai_socktype = SOCK_STREAM;

    // getting result address info structure
    if( (gai_res = getaddrinfo(NULL, PORT, &addrinfo_hints, &addrinfo_res)) != 0 ) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(gai_res));
        return FAILURE;
    }

    // loop through address info results and bind to the first we can
    for(ai_ptr = addrinfo_res; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {

        // create new server passive socket
        if( (ps_fd = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol)) < 0 ) {
            fprintf(stderr, "server passive socket: %s\n", strerror(errno));
            continue;
        }

        // set passive socket to reuse host address and port
        if( setsockopt(ps_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0 ) {
            fprintf(stderr, "setsockopt: %s\n", strerror(errno));
            return FAILURE;
        }

        // bind passive socket to (address, port) defined in address info structure
        if( bind(ps_fd, ai_ptr->ai_addr, ai_ptr->ai_addrlen) < 0 ) {
            close(ps_fd);
            fprintf(stderr, "bind: %s\n", strerror(errno));
            continue;
        }

        break;
    }

    if(ai_ptr == NULL) {
        fprintf(stderr, "server passive socket: failed to be created or binded\n");
        return FAILURE;
    }

    freeaddrinfo(addrinfo_res);

    *res_fd = ps_fd; // return created passive socket file descriptor

    print_socket_address(ps_fd, PASSIVE_SOCKET);

    return SUCCESS;
}

result_t listen_connections(int ps_fd) {

    if( listen(ps_fd, BACKLOG) < 0 ) {
        fprintf(stderr, "listen: %s\n", strerror(errno));
        return FAILURE;
    }

    printf("Start listening...\n");

    return SUCCESS;
}

/**
 * function is looping infinitely and waiting
 * for new incoming client connections
 */
int wait_for_connections(int ps_fd) {

    printf("Waiting for new connections on the main thread...\n");

    while(1) {
        accept_new_connection(ps_fd);
    }
}

int accept_new_connection(int ps_fd) {

    int cs_fd; // connection socket file descriptor
    struct sockaddr sockaddr;
    socklen_t sockaddrlen = sizeof(sockaddr);

    if( (cs_fd = accept(ps_fd, &sockaddr, &sockaddrlen)) < 0 ) {
        if(errno == EINTR) return CONTINUE;
        fprintf(stderr, "accept: %s\n", strerror(errno));
        return FAILURE;
    }

    print_socket_address(cs_fd, CONNECTION_SOCKET);

    /*  char *ip_address;
        int port;
        get_address_and_port_from_sockaddr(&sockaddr, &ip_address, &port);

        printf("Socket %d connected to %s:%d\n", cs_fd, ip_address, port);
     */

    handle_communication(cs_fd);

    return SUCCESS;
}

int handle_communication(int cs_fd) {

    while(1) echo(cs_fd);
}

int end_server() {


    return SUCCESS;
}

result_t echo(int cs_fd) {

    char rbuf[100];
    int n_recv; // number of bytes received
    int n_sent; // number of bytes sent

    if( (n_recv = recv(cs_fd, rbuf, sizeof(rbuf) -1, 0)) < 0) {
        perror("recv");
        return FAILURE;
    }

    rbuf[n_recv] = '\0';

    printf("server: received '%s'\n", rbuf);

    if( (n_sent = send(cs_fd, rbuf, strlen(rbuf), 0)) < 0) {
        perror("send");
        return FAILURE;
    }

    return SUCCESS;
}