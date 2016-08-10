//
// Created by Michal Ziobro on 08/08/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_THREADS_MANAGER_H
#define REMOTECONTROLLERAPI_SERVER_THREADS_MANAGER_H

#include <ntsid.h>
#include "../common/types.h"
#include "../networking/passive_connection.h"
#include "../networking/datagram.h"
#include "runner.h"

struct threads_manager;
typedef struct threads_manager threads_manager_t;

// threads manager operations
result_t threads_manager_init(threads_manager_t **threads_manager, size_t max_limit);
void threads_manager_set_max_limit(threads_manager_t *threads_manager, size_t max_limit);
size_t threads_manager_max_limit(threads_manager_t *threads_manager);
size_t threads_manager_count(threads_manager_t *threads_manager);
pthread_t *threads_manager_threads(threads_manager_t *threads_manager);
result_t wait_for_thread(threads_manager_t *threads_manager, runner_t runner, runner_attr_t runner_attr);
result_t wait_for_connection_thread(threads_manager_t *threads_manager, connection_handler_t conn_handler, sock_fd_t conn_sock_fd);
result_t wait_for_datagram_thread(threads_manager_t *threads_manager, datagram_handler_t datagram_handler, sock_fd_t sock_fd, const struct sockaddr *peer_addr, const char *datagram);
result_t timed_wait_for_thread(threads_manager_t *threads_manager, const int ms_timeout, runner_t runner, runner_attr_t runner_attr);
result_t timed_wait_for_connection_thread(threads_manager_t *threads_manager, const int ms_timeout, connection_handler_t conn_handler, sock_fd_t conn_sock_fd);
result_t timed_wait_for_datagram_thread(threads_manager_t *threads_manager, const int ms_timeout, datagram_handler_t datagram_handler, sock_fd_t sock_fd, const struct sockaddr *peer_addr, const char *datagram);
result_t threads_manager_remove_thread(threads_manager_t *threads_manager, pthread_t thread);
void threads_manager_free(threads_manager_t *threads_manager);

#endif //REMOTECONTROLLERAPI_SERVER_THREADS_MANAGER_H
