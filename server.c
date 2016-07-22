/*
 * Created by Michal Ziobro on 21/07/2016.
 */

#include "server.h"
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

int start_server(void) {

    int ps_fd;

    printf("Starting new server...\n");

    if(create_passive_socket(&ps_fd) == FAILURE ) {
        fprintf(stderr, "create_passive_socket: faild!");
        return FAILURE;
    }

    listen_connections(ps_fd);

    close(ps_fd);

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
        fprintf(stderr, "server passive socket: failed to be created or binded\n");
        return FAILURE;
    }

    freeaddrinfo(addrinfo_res);

    *res_fd = ps_fd; // return created passive socket file descriptor

    print_socket_address(ps_fd, PASSIVE_SOCKET);

    return SUCCESS;
}

int listen_connections(int ps_fd) {

    if( listen(ps_fd, BACKLOG) < 0 ) {
        fprintf(stderr, "listen: %s\n", strerror(errno));
        return FAILURE;
    }

    printf("Start listening...\n");

    return SUCCESS;
}

int accept_new_connection(void) {

    return SUCCESS;
}

int end_server() {


    return SUCCESS;
}

int print_socket_address(int sockfd, socket_type_t socket_type) {

    char *ip_address; // address (passive) socket was binded to
    int port; // port (passive) socket was binded to

    if(get_address_and_port_from_sockfd(sockfd, &ip_address, &port) == FAILURE) {
        fprintf(stderr, "get_address_and_port_from_sockfd: faild!");
        return FAILURE;
    }

    switch(socket_type)
    {
        case PASSIVE_SOCKET:
            printf("Created passive socket %d binded to %s:%d\n", sockfd, ip_address, port);
            break;
        case CONNECTION_SOCKET:
            printf("Socket %d binded to %s:%d\n", sockfd, ip_address, port);
            break;
        default:
            free(ip_address);
            fprintf(stderr, "Incorrect socket type!");
            return FAILURE;
    }

    free(ip_address);

    return SUCCESS;
}

/**
 * function retrieves ip address and port for given socket file descriptor
 */
int get_address_and_port_from_sockfd(int sockfd, char **ip_address, int *port) {

    struct sockaddr sockaddr;
    socklen_t sockaddrlen = sizeof(sockaddr);

    if(getsockname(sockfd, &sockaddr, &sockaddrlen) < 0) {
        fprintf(stderr, "getsockname: %s\n", strerror(errno));
        return FAILURE;
    }

    return get_address_and_port_from_sockaddr(&sockaddr, ip_address, port);
}

/**
 * function unwrap ip address and port from addrinfo structure
 */
int get_address_and_port_from_addrinfo(const struct addrinfo *addrinfo, char **ip_address, int *port) {

    return get_address_and_port_from_sockaddr((struct sockaddr *)addrinfo->ai_addr, ip_address, port);
}

/**
 * function unwrap ip address and port from sockaddr structure
 */
int get_address_and_port_from_sockaddr(const struct sockaddr *sockaddr, char **ip_address, int *port) {

    *ip_address = malloc(INET6_ADDRSTRLEN);

    // converting network address to presentation address
    if(inet_ntop(sockaddr->sa_family, get_in_addr(sockaddr), *ip_address, sizeof(*ip_address)) == NULL) {
        fprintf(stderr, "inet_ntop: %s\n", strerror(errno));
        return FAILURE;
    }

    // converting network port to host port
    *port = ntohs(get_in_port(sockaddr));

    return SUCCESS;
}

/**
 * function unwrap in_addr or in6_addr structure from
 * sockaddr structure depending on address family
 * AF_INET or AF_INET6
 */
void *get_in_addr(const struct sockaddr *sa) {

    if( sa->sa_family == AF_INET) // IPv4 address
        return &(((struct sockaddr_in*)sa)->sin_addr);
    // else IPv6 address
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/**
 * function unwrap in_port from sockaddr structure
 * depending on address family AF_INET or AF_INET6
 */
in_port_t get_in_port(const struct sockaddr *sa)
{
    if( sa->sa_family == AF_INET ) // IPv4 address
        return (((struct sockaddr_in*)sa)->sin_port);
    // else IPv6 address
    return (((struct sockaddr_in6*)sa)->sin6_port);
}
