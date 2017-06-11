//
// Created by Michal Ziobro on 09/08/2016.
//
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "runner.h"

// connection thread runner
/**
 * wrapping all arguments needed by connection thread runner to handle connection
 * into single structure to make them passable to thread's start routine
 */
struct conn_thread_runner_attr {
    connection_handler_t conn_handler;
    server_info_t *server_info;
    sock_fd_t conn_sock_fd;
    callback_t callback;
    callback_attr_t callback_attr;
};
/**
 * runner function used to wrap invocation of connection handler
 * with its params in order to make it passable as thread's start routine
 */
runner_res_t connection_thread_runner(conn_thread_runner_attr_t *attr) {
    result_t res = attr->conn_handler(attr->server_info, attr->conn_sock_fd);
    if(attr->callback != NULL)
        attr->callback(attr->callback_attr, (void *) &res);

    switch(res) {
        case FAILURE:
            fprintf(stderr, "connection_handler: failed!\n");
            // publish connection error event
            server_info_connection_error_event(attr->server_info, attr->conn_sock_fd, CONN_ERROR_HANDLER, "connection_handler: failed!\n");
            break;
        case CLOSED:
            printf("connection_handler: closed!\n");
            // publish client disconnecting event
            server_info_client_disconnecting_event(attr->server_info, attr->conn_sock_fd);
            break;
        default:
            break;
    }

    if( close(attr->conn_sock_fd) < 0 ) {
        fprintf(stderr, "close: %s\n", strerror(errno));
        server_info_connection_error_event(attr->server_info, attr->conn_sock_fd, CONN_ERROR_CLOSE, strerror(errno));
    }
    conn_thread_runner_attr_free(attr);
    return NULL;
}

// datagram thread runner
/**
 * wrapping all arguments needed by datagram thread runner to handle datagram request
 * into single structure to make them passable to thread's start routine
 */
struct datagram_thread_runner_attr {
    datagram_handler_t datagram_handler;
    sock_fd_t sock_fd;
    struct sockaddr *peer_addr;
    char *datagram;
    callback_t callback;
    callback_attr_t callback_attr;
};
/**
 * runner function used to wrap invocation of datagram handler
 * with its params in order to make it passable as thread's start routine
 */
runner_res_t datagram_thread_runner(datagram_thread_runner_attr_t *attr) {
    result_t res = attr->datagram_handler(attr->sock_fd, attr->peer_addr, attr->datagram);
    if(attr->callback != NULL)
        attr->callback(attr->callback_attr, (void *) &res);
    datagram_thread_runner_attr_free(attr);
    return NULL;
}

// connection thread runner attribute operations
result_t conn_thread_runner_attr_init(conn_thread_runner_attr_t **runner_attr) {

    if( ( *runner_attr = malloc(sizeof(conn_thread_runner_attr_t)) ) == NULL ) {
        fprintf(stderr, "malloc: failed to allocate conn_thread_runner_attr.\n");
        return FAILURE;
    }
    return SUCCESS;
}
void conn_thread_runner_attr_fill(conn_thread_runner_attr_t *runner_attr, connection_handler_t conn_handler, server_info_t *server_info, sock_fd_t conn_sock_fd, callback_t callback, callback_attr_t callback_attr) {

    memset(runner_attr, 0, sizeof(*runner_attr));
    runner_attr->conn_handler = conn_handler;
    runner_attr->server_info = server_info;
    runner_attr->conn_sock_fd = conn_sock_fd;
    runner_attr->callback = callback;
    runner_attr->callback_attr = callback_attr;
}
void conn_thread_runner_attr_free(conn_thread_runner_attr_t *runner_attr) {

    free(runner_attr);
}

// datagram thread runner attribute operations
result_t datagram_thread_runner_attr_init(datagram_thread_runner_attr_t **runner_attr) {

    if( (*runner_attr = malloc(sizeof(datagram_thread_runner_attr_t)) ) == NULL) {
        fprintf(stderr, "malloc: failed to allocate datagram_thread_runner_attr.\n");
        return FAILURE;
    }
    return SUCCESS;
}
void datagram_thread_runner_attr_fill(datagram_thread_runner_attr_t *runner_attr, datagram_handler_t datagram_handler, sock_fd_t sock_fd, struct sockaddr *peer_addr, char *datagram, callback_t callback, callback_attr_t callback_attr) {

    memset(runner_attr, 0, sizeof(*runner_attr));
    runner_attr->datagram_handler = datagram_handler;
    runner_attr->sock_fd = sock_fd;
    runner_attr->peer_addr = malloc(sizeof(*peer_addr)); memcpy(runner_attr->peer_addr, peer_addr, sizeof(*peer_addr));
    runner_attr->datagram = malloc(sizeof(char)*(strlen(datagram)+1));  memcpy(runner_attr->datagram, datagram, strlen(datagram)+1);
    runner_attr->callback = callback;
    runner_attr->callback_attr = callback_attr;
}
void datagram_thread_runner_attr_free(datagram_thread_runner_attr_t *runner_attr) {

    free(runner_attr->peer_addr);
    free(runner_attr->datagram);
    free(runner_attr);
}