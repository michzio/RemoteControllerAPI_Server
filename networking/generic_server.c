//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include "generic_server.h"

struct server_info {
    char *port;
    char *ip;
    sock_fd_t sockfd;
};

// server_info_t operations
void server_info_init(server_info_t **info) {

    *info = malloc(sizeof(server_info_t));
}

void server_info_set_port(server_info_t *info, const char *port) {
    info->port = port;
}

void server_info_set_ip(server_info_t *info, const char *ip) {

    info->ip = ip;
}

void server_info_set_sock(server_info_t *info, const sock_fd_t sockfd) {

    info->sockfd = sockfd;
}

result_t server_info_fill(server_info_t *info, const sock_fd_t sockfd) {

    info->sockfd = sockfd;

    char *ip_address;
    int port_number;

    if(get_current_address_and_port(sockfd, &ip_address, &port_number) == FAILURE) {
        fprintf(stderr, "get_current_address_and_port: faild!\n");
        free(info->ip);
        return FAILURE;
    }

    // converting integer port number into string equivalent
    size_t port_len = (size_t)((ceil(log10(port_number))+1)*sizeof(char));
    char *port = malloc(port_len);
    snprintf(port, port_len, "%d", port_number);

    info->ip = ip_address;
    info->port = port;

    return SUCCESS;
}

const char *server_info_port(const server_info_t *info) {

    return info->port;
}

const char *server_info_ip(const server_info_t *info) {

    return info->ip;
}

const sock_fd_t server_info_sock(const server_info_t *info) {

    return info->sockfd;
}

void server_info_free(server_info_t *info) {

    free(info); info = NULL;
}

result_t create_stream_server(server_info_t *info, stream_server_loop_t server_loop, connection_handler_t conn_handler) {

    int ps_fd; // passive socket file descriptor

    printf("Creating new stream server...\n");

    if(create_stream_pasv_sock(server_info_port(info), &ps_fd) == FAILURE) {
        fprintf(stderr, "create_stream_pasv_sock: failed!\n");
        return FAILURE;
    }

    // for information & debugging purposes
    if(print_socket_address(ps_fd, PASSIVE_SOCKET) == FAILURE) {
        fprintf(stderr, "print_socket_address: failed!\n");
        return FAILURE;
    }

    if(server_info_fill(info, ps_fd) == FAILURE) {
        fprintf(stderr, "server_info_fill: failed!\n");
        return FAILURE;
    }

    if(listen_connections(ps_fd) == FAILURE) {
        fprintf(stderr, "listen_connections: failed!\n");
        return FAILURE;
    }

    if(server_loop(ps_fd, conn_handler) == FAILURE) {
        fprintf(stderr, "server_loop: failed!\n");
        return FAILURE;
    }

    if(close(ps_fd) < 0) {
        fprintf(stderr, "close: %s\n", strerror(errno));
        return FAILURE;
    }

    return SUCCESS;
}

result_t create_datagram_server(server_info_t *info, datagram_server_loop_t server_loop, datagram_handler_t datagram_handler) {

    int ps_fd; // passive socket file descriptor

    printf("Creating new datagram server...\n");

    if(create_datagram_pasv_sock(server_info_port(info), &ps_fd) == FAILURE) {
        fprintf(stderr, "create_datagram_pasv_sock: failed!\n");
        return FAILURE;
    }

    // for information & debugging purposes
    if(print_socket_address(ps_fd, PASSIVE_SOCKET) == FAILURE) {
        fprintf(stderr, "print_socket_address: failed!\n");
        return FAILURE;
    }

    if(server_info_fill(info, ps_fd) == FAILURE) {
        fprintf(stderr, "server_info_fill: failed!\n");
        return FAILURE;
    }

    if(server_loop(ps_fd, datagram_handler) == FAILURE) {
        fprintf(stderr, "server_loop: failed!\n");
        return FAILURE;
    }

    if(close(ps_fd) < 0) {
        fprintf(stderr, "close: %s\n", strerror(errno));
        return FAILURE;
    }

    return SUCCESS;
}