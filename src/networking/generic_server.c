//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include "networking/generic_server.h"
#include "linked_list/linked_list.h"


result_t create_stream_server(server_info_t *info, stream_server_loop_t server_loop, connection_handler_t conn_handler) {

    int ps_fd; // passive socket file descriptor

    printf("Creating new stream server...\n");

    if(create_stream_pasv_sock(server_info_port(info), &ps_fd) == FAILURE) {
        fprintf(stderr, "create_stream_pasv_sock: failed!\n");
        server_info_server_error_event(info, SERVER_ERROR_CREATE_PASV_SOCK, "create_stream_pasv_sock: failed!");
        return FAILURE;
    }

    // for information & debugging purposes
    if(print_socket_address(ps_fd, PASSIVE_SOCKET) == FAILURE) {
        fprintf(stderr, "print_socket_address: failed!\n");
        server_info_server_error_event(info, SERVER_ERROR_PRINT_ADDRESS, "print_socket_address: failed!");
        return FAILURE;
    }

    if(server_info_fill(info, ps_fd) == FAILURE) {
        fprintf(stderr, "server_info_fill: failed!\n");
        server_info_server_error_event(info, SERVER_ERROR_INFO_FILL, "server_info_fill: failed!");
        return FAILURE;
    }

    if(listen_connections(ps_fd) == FAILURE) {
        fprintf(stderr, "listen_connections: failed!\n");
        server_info_server_error_event(info, SERVER_ERROR_LISTEN_CONN, "listen_connections: failed!");
        return FAILURE;
    }

    server_info_server_start_event(info);

    // set passive socket to non-blocking mode
    // in order to enable lazy server closing
    // and ordered force shut down of server
    fcntl(ps_fd, F_SETFL, O_NONBLOCK);

    switch(server_loop(info, conn_handler)) {
        case FAILURE:
            fprintf(stderr, "server_loop: failed!\n");
            server_info_server_error_event(info, SERVER_ERROR_LOOP, "server_loop: failed!");
            return FAILURE;
        case FORCE_CLOSED:
        default:
            break;
    }

    if(close(ps_fd) < 0) {
        fprintf(stderr, "close: %s\n", strerror(errno));
        server_info_server_error_event(info, SERVER_ERROR_CLOSE, strerror(errno));
        return FAILURE;
    }

    server_info_server_end_event(info);

    return SUCCESS;
}

result_t create_datagram_server(server_info_t *info, datagram_server_loop_t server_loop, datagram_handler_t datagram_handler) {

    int ps_fd; // passive socket file descriptor

    printf("Creating new datagram server...\n");

    if(create_datagram_pasv_sock(server_info_port(info), &ps_fd) == FAILURE) {
        fprintf(stderr, "create_datagram_pasv_sock: failed!\n");
        server_info_server_error_event(info, SERVER_ERROR_CREATE_PASV_SOCK, "create_datagram_pasv_sock: failed!");
        return FAILURE;
    }

    // for information & debugging purposes
    if(print_socket_address(ps_fd, PASSIVE_SOCKET) == FAILURE) {
        fprintf(stderr, "print_socket_address: failed!\n");
        server_info_server_error_event(info, SERVER_ERROR_PRINT_ADDRESS, "print_socket_address: failed!");
        return FAILURE;
    }

    if(server_info_fill(info, ps_fd) == FAILURE) {
        fprintf(stderr, "server_info_fill: failed!\n");
        server_info_server_error_event(info, SERVER_ERROR_INFO_FILL, "server_info_fill: failed!");
        return FAILURE;
    }

    server_info_server_start_event(info);

    // set passive socket to non-blocking mode
    // in order to enable lazy server closing
    fcntl(ps_fd, F_SETFL, O_NONBLOCK);

    if(server_loop(info, datagram_handler) == FAILURE) {
        fprintf(stderr, "server_loop: failed!\n");
        server_info_server_error_event(info, SERVER_ERROR_LOOP, "server_loop: failed!");
        return FAILURE;
    }

    printf("Server loop stopped!\n");

    if(close(ps_fd) < 0) {
        fprintf(stderr, "close: %s\n", strerror(errno));
        server_info_server_error_event(info, SERVER_ERROR_CLOSE, strerror(errno));
        return FAILURE;
    }

    printf("Server passive socket: %d closed.\n", ps_fd);

    server_info_server_end_event(info);

    return SUCCESS;
}