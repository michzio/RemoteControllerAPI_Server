/*
 * Created by Michal Ziobro on 21/07/2016.
 */

#include "server.h"
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>

int start_server(void) {

    printf("Starting new server...\n");

    return SUCCESS;
}

int create_passive_socket(void) {

    int ps_fd; // passive socket file descriptor
    struct addrinfo addrinfo_hints, *addrinfo_res, *ai_ptr; // address info structures holding host address information
    int gai_res;

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
        if( (ps_fd = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol)) < 0) {
            fprintf(stderr, "server passive socket: %s\n", strerror(errno));
            continue;
        }

        // set passive socket to reuse host address and port

    }

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