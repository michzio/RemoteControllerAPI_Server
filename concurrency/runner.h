//
// Created by Michal Ziobro on 09/08/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_RUNNER_H
#define REMOTECONTROLLERAPI_SERVER_RUNNER_H

#include "../networking/passive_connection.h"
#include "../networking/datagram.h"

// runner type definition
typedef void * runner_attr_t;
typedef void * runner_res_t;
typedef runner_res_t (*runner_t)(runner_attr_t);
typedef void (*runner_res_handler_t) (runner_res_t);

// connection thread runner
struct conn_thread_runner_attr;
typedef struct conn_thread_runner_attr conn_thread_runner_attr_t;
runner_res_t connection_thread_runner(conn_thread_runner_attr_t *attr);

result_t conn_thread_runner_attr_init(conn_thread_runner_attr_t **runner_attr);
void conn_thread_runner_attr_fill(conn_thread_runner_attr_t *runner_attr, connection_handler_t conn_handler, sock_fd_t conn_sock_fd, callback_t callback, callback_attr_t callback_attr);
void conn_thread_runner_attr_free(conn_thread_runner_attr_t *runner_attr);

// datagram thread runner
struct datagram_thread_runner_attr;
typedef struct datagram_thread_runner_attr datagram_thread_runner_attr_t;
runner_res_t datagram_thread_runner(datagram_thread_runner_attr_t *attr);

result_t datagram_thread_runner_attr_init(datagram_thread_runner_attr_t **runner_attr);
void datagram_thread_runner_attr_fill(datagram_thread_runner_attr_t *runner_attr, datagram_handler_t datagram_handler, sock_fd_t sock_fd, struct sockaddr *peer_addr, char *datagram, callback_t callback, callback_attr_t callback_attr);
void datagram_thread_runner_attr_free(datagram_thread_runner_attr_t *runner_attr);

#endif //REMOTECONTROLLERAPI_SERVER_RUNNER_H
