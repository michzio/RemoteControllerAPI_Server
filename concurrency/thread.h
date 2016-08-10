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
#include "thread_pool/task_queue.h"

// create thread: generic thread, connection thread, datagram thread
pthread_t thread_create(runner_t runner, runner_attr_t runner_attr);
pthread_t connection_thread(connection_handler_t conn_handler, sock_fd_t conn_sock_fd);
pthread_t connection_thread_with_callback(connection_handler_t conn_handler, sock_fd_t conn_sock_fd, callback_t callback, callback_attr_t callback_attr);
pthread_t datagram_thread(datagram_handler_t datagram_handler, sock_fd_t sock_fd, struct sockaddr *peer_addr, char *datagram);
pthread_t datagram_thread_with_callback(datagram_handler_t datagram_handler, sock_fd_t sock_fd, struct sockaddr *peer_addr, char *datagram, callback_t callback, callback_attr_t callback_attr);

#endif //REMOTECONTROLLERAPI_SERVER_THREAD_H
