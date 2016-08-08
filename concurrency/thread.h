//
// Created by Michal Ziobro on 27/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_THREAD_H
#define REMOTECONTROLLERAPI_SERVER_THREAD_H

#define _REENTRANT
#include <pthread.h>

#include "../common/address_helper.h"
#include "../networking/datagram.h"
#include "../networking/passive_connection.h"

// connection thread
struct conn_thread_runner_attr;
typedef struct conn_thread_runner_attr conn_thread_runner_attr_t;

void *connection_thread_runner(conn_thread_runner_attr_t *attr);
result_t connection_thread(connection_handler_t conn_handler, sock_fd_t conn_sock_fd);

// datagram thread
struct datagram_thread_runner_attr;
typedef struct datagram_thread_runner_attr datagram_thread_runner_attr_t;

void *datagram_thread_runner(datagram_thread_runner_attr_t *attr);
result_t datagram_thread(datagram_handler_t datagram_handler, sock_fd_t sock_fd, const struct sockaddr *peer_addr, const char *datagram);

#endif //REMOTECONTROLLERAPI_SERVER_THREAD_H
