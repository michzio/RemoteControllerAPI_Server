//
// Created by Michal Ziobro on 09/08/2016.
//
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "runner.h"

// connection thread runner
/**
 * wrapping all arguments needed by connection thread runner to handle connection
 * into single structure to make them passable to thread's start routine
 */
struct conn_thread_runner_attr {
    connection_handler_t conn_handler;
    sock_fd_t conn_sock_fd;
    callback_t callback;
    callback_attr_t callback_attr;
};
/**
 * runner function used to wrap invocation of connection handler
 * with its params in order to make it passable as thread's start routine
 */
runner_res_t connection_thread_runner(conn_thread_runner_attr_t *attr) {
    result_t res = attr->conn_handler(attr->conn_sock_fd);
    if(attr->callback != NULL)
        attr->callback(attr->callback_attr, (void *) &res);
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

// managed thread runner
/**
 * wrapping runner_t and runner_attr_t into single structure to enable usage with
 * threads managed by threads manager
 */
struct managed_thread_runner_attr {
    runner_t runner;
    runner_attr_t runner_attr;
    callback_t callback;
    callback_attr_t callback_attr;
};
/**
 * managed thread runner function used to wrap invocation of generic runner
 * function with runner_attr as its only param to make possible usage
 * of runner_t and runner_attr_t with managed threads and threads manager object
 */
runner_res_t managed_thread_runner(managed_thread_runner_attr_t *attr) {

    runner_res_t runner_res = attr->runner(attr->runner_attr);
    if(attr->callback != NULL)
        attr->callback(attr->callback_attr, (void *) runner_res);
    managed_thread_runner_attr_free(attr);
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
void conn_thread_runner_attr_fill(conn_thread_runner_attr_t *runner_attr, connection_handler_t conn_handler, sock_fd_t conn_sock_fd, callback_t callback, callback_attr_t callback_attr) {

    memset(runner_attr, 0, sizeof(*runner_attr));
    runner_attr->conn_handler = conn_handler;
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
    runner_attr->peer_addr = peer_addr;
    runner_attr->datagram = datagram;
    runner_attr->callback = callback;
    runner_attr->callback_attr = callback_attr;
}
void datagram_thread_runner_attr_free(datagram_thread_runner_attr_t *runner_attr) {

    free(runner_attr->peer_addr);
    free(runner_attr->datagram);
    free(runner_attr);
}

// managed thread runner attributes operations
result_t managed_thread_runner_attr_init(managed_thread_runner_attr_t **managed_thread_runner_attr) {

    if( ( *managed_thread_runner_attr = malloc(sizeof(managed_thread_runner_attr_t)) ) == NULL ) {
        fprintf(stderr, "malloc: failed to allocate managed_thread_runner_attr.\n");
        return FAILURE;
    }
    return SUCCESS;
}
void managed_thread_runner_attr_fill(managed_thread_runner_attr_t *managed_thread_runner_attr, runner_t runner, runner_attr_t runner_attr, callback_t callback, callback_attr_t callback_attr) {

    memset(managed_thread_runner_attr, 0, sizeof(*managed_thread_runner_attr));
    managed_thread_runner_attr->runner = runner;
    managed_thread_runner_attr->runner_attr = runner_attr;
    managed_thread_runner_attr->callback = callback;
    managed_thread_runner_attr->callback_attr = callback_attr;
}
void managed_thread_runner_attr_free(managed_thread_runner_attr_t *managed_thread_runner_attr) {

    free(managed_thread_runner_attr);
}