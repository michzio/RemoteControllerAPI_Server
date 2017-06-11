//
// Created by Michal Ziobro on 08/08/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_THREADS_MANAGER_H
#define REMOTECONTROLLERAPI_SERVER_THREADS_MANAGER_H

#include <stdio.h>
#include "../../common/types.h"
#include "../networking/passive_connection.h"
#include "../networking/datagram.h"
#include "../../concurrency/threads/generic_threads_manager.h"

result_t wait_for_connection_thread(threads_manager_t *threads_manager, connection_handler_t conn_handler, server_info_t *server_info, sock_fd_t conn_sock_fd);
result_t wait_for_datagram_thread(threads_manager_t *threads_manager, datagram_handler_t datagram_handler, sock_fd_t sock_fd, struct sockaddr *peer_addr, char *datagram);
result_t timed_wait_for_connection_thread(threads_manager_t *threads_manager, const int ms_timeout, connection_handler_t conn_handler, server_info_t *server_info, sock_fd_t conn_sock_fd);
result_t timed_wait_for_datagram_thread(threads_manager_t *threads_manager, const int ms_timeout, datagram_handler_t datagram_handler, sock_fd_t sock_fd, struct sockaddr *peer_addr, char *datagram);

#endif //REMOTECONTROLLERAPI_SERVER_THREADS_MANAGER_H
