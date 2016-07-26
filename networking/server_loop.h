//
// Created by Michal Ziobro on 26/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_SERVER_LOOP_H
#define REMOTECONTROLLERAPI_SERVER_SERVER_LOOP_H

#include "../common/address_helper.h"
#include "passive_socket.h"
#include "passive_connection.h"
#include "datagram.h"

typedef result_t (*stream_server_loop_t) (sock_fd_t, connection_handler_t);
typedef result_t (*datagram_server_loop_t) (sock_fd_t, datagram_handler_t);

// TCP
result_t iterative_stream_server_loop(sock_fd_t ps_fd, connection_handler_t conn_handler);
result_t concurrent_stream_server_loop(sock_fd_t ps_fd, connection_handler_t conn_handler);
result_t pseudo_concurrent_stream_server_loop(sock_fd_t ps_fd, connection_handler_t conn_handler);

// UDP
result_t iterative_datagram_server_loop(sock_fd_t ps_fd, datagram_handler_t datagram_handler);
result_t concurrent_datagram_server_loop(sock_fd_t ps_fd, datagram_handler_t datagram_handler);

#endif //REMOTECONTROLLERAPI_SERVER_SERVER_LOOP_H
