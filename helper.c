//
// Created by Michal Ziobro on 24/07/2016.
//


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "helper.h"

result_t print_socket_address(int sockfd, socket_type_t socket_type) {

    char *ip_address; // address (passive) socket was binded to
    int port; // port (passive) socket was binded to

    switch(socket_type)
    {
        case PASSIVE_SOCKET:
            if(get_current_address_and_port(sockfd, &ip_address, &port) == FAILURE) {
                fprintf(stderr, "get_current_address_and_port: faild!");
                free(ip_address);
                return FAILURE;
            }
            printf("Created passive socket %d binded to %s:%d\n", sockfd, ip_address, port);
            break;
        case CONNECTION_SOCKET:
            if(get_peer_address_and_port(sockfd, &ip_address, &port) == FAILURE) {
                fprintf(stderr, "get_peer_address_and_port: faild!");
                free(ip_address);
                return FAILURE;
            }
            printf("Socket %d connected to %s:%d\n", sockfd, ip_address, port);
            break;
        default:
            fprintf(stderr, "Incorrect socket type!");
            free(ip_address);
            return FAILURE;
    }

    free(ip_address);
    return SUCCESS;
}

/**
 * function retrieves current ip address and port
 * socket is bound to for given socket file descriptor
 */
result_t get_current_address_and_port(int sockfd, char **ip_address, int *port) {

    struct sockaddr sockaddr;
    socklen_t sockaddrlen = sizeof(sockaddr);

    if(getsockname(sockfd, &sockaddr, &sockaddrlen) < 0) {
        fprintf(stderr, "getsockname: %s\n", strerror(errno));
        return FAILURE;
    }

    return get_address_and_port_from_sockaddr(&sockaddr, ip_address, port);
}

/**
 * function retrieves peer ip address and port
 * socket is connected to for given socket file descriptor
 */
result_t get_peer_address_and_port(int sockfd, char **ip_address, int *port) {

    struct sockaddr sockaddr;
    socklen_t sockaddrlen = sizeof(sockaddr);

    if(getpeername(sockfd, &sockaddr, &sockaddrlen) < 0) {
        fprintf(stderr, "getpeername: %s\n", strerror(errno));
        return FAILURE;
    }

    return get_address_and_port_from_sockaddr(&sockaddr, ip_address, port);
}

/**
 * function unwrap ip address and port from addrinfo structure
 */
result_t get_address_and_port_from_addrinfo(const struct addrinfo *addrinfo, char **ip_address, int *port) {

    return get_address_and_port_from_sockaddr((struct sockaddr *)addrinfo->ai_addr, ip_address, port);
}

/**
 * function unwrap ip address and port from sockaddr structure
 */
result_t get_address_and_port_from_sockaddr(const struct sockaddr *sockaddr, char **ip_address, int *port) {

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
