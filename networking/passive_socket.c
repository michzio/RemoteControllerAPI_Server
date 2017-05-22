//
// Created by Michal Ziobro on 25/07/2016.
//

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "passive_socket.h"

/**
 * Function creates new passive socket binded
 * to current server on given PORT. Socket can
 * be either TCP or UDP depending on sock_type.
 */
static result_t create_passive_socket(const char *port, sock_type_t sock_type, sock_fd_t *pasv_sock_fd) {

    int ps_fd; // passive socket file descriptor
    struct addrinfo addrinfo_hints, *addrinfo_res, *ai_ptr; // address info structures holding host address information
    int gai_res; // getaddrinfo() result value
    int optval_yes = 1; // used in setsockopt to set option value to yes
    int optval_no = 0;  // used in setsockopt to set option value to no

    // populating address info hints for getaddrinfo()
    memset(&addrinfo_hints, 0, sizeof(addrinfo_hints));
    addrinfo_hints.ai_flags = AI_PASSIVE; // current host IP
    addrinfo_hints.ai_family = AF_UNSPEC; // AF_INET to force IPv4, AF_INET6 to force IPv6
    addrinfo_hints.ai_socktype = (int) sock_type;

    // getting result address info structure
    if( (gai_res = getaddrinfo(NULL, port, &addrinfo_hints, &addrinfo_res)) != 0 ) {
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
        if( setsockopt(ps_fd, SOL_SOCKET, SO_REUSEADDR, &optval_yes, sizeof(optval_yes)) < 0 ) {
            fprintf(stderr, "setsockopt SO_REUSEADDR: %s\n", strerror(errno));
            return FAILURE;
        }

        if( (ai_ptr->ai_family == AF_INET6) && setsockopt(ps_fd, IPPROTO_IPV6, IPV6_V6ONLY, &optval_no, sizeof(optval_no)) < 0 ) {
            fprintf(stderr, "setsockopt IPV6_V6ONLY: %s\n", strerror(errno));
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

    *pasv_sock_fd = ps_fd; // return created passive socket file descriptor

    return SUCCESS;
}

result_t create_stream_pasv_sock(const char *port, sock_fd_t *pasv_sock_fd) {
    return create_passive_socket(port, SOCK_STREAM, pasv_sock_fd);
}
result_t create_datagram_pasv_sock(const char *port, sock_fd_t *pasv_sock_fd) {
    return create_passive_socket(port, SOCK_DGRAM, pasv_sock_fd);
}

