/*
 * Created by Michal Ziobro on 21/07/2016.
 */

#include "server.h"
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int start_server(void) {

    int ps_fd;

    printf("Starting new server...\n");

    if(create_passive_socket(&ps_fd) == FAILURE ) {
        fprintf(stderr, "create_passive_socket: faild!");
        return FAILURE;
    }

    return SUCCESS;
}

int create_passive_socket(int *res_fd) {

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
        fprintf(stderr, "server passive socket: failed to create or bind\n");
        return FAILURE;
    }

    freeaddrinfo(addrinfo_res);

    *res_fd = ps_fd; // return created passive socket file descriptor

    return SUCCESS;
}

int listen_connections(void) {

    return SUCCESS;
}

int accept_new_connection(void) {

    return SUCCESS;
}

int end_server() {


    return SUCCESS;
}