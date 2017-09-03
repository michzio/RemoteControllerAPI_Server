//
// Created by Michal Ziobro on 26/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_SERVER_LOOP_H
#define REMOTECONTROLLERAPI_SERVER_SERVER_LOOP_H

#include "server_info.h"
#include "datagram.h"
#include "passive_connection.h"

typedef result_t (*stream_server_loop_t) (server_info_t *, connection_handler_t);
typedef result_t (*datagram_server_loop_t) (server_info_t *, datagram_handler_t);

// TCP
result_t iterative_stream_server_loop(server_info_t *server_info, connection_handler_t conn_handler);
result_t concurrent_stream_server_loop(server_info_t *server_info, connection_handler_t conn_handler);
result_t managed_concurrent_stream_server_loop(server_info_t *server_info, connection_handler_t conn_handler);
result_t thread_pool_stream_server_loop(server_info_t *server_info, connection_handler_t conn_handler);
result_t pseudo_concurrent_stream_server_loop(server_info_t *server_info, request_handler_t request_handler);

// UDP
result_t iterative_datagram_server_loop(server_info_t *server_info, datagram_handler_t datagram_handler);
result_t concurrent_datagram_server_loop(server_info_t *server_info, datagram_handler_t datagram_handler);
result_t managed_concurrent_datagram_server_loop(server_info_t *server_info, datagram_handler_t datagram_handler);
result_t thread_pool_datagram_server_loop(server_info_t *server_info, datagram_handler_t datagram_handler);

#endif //REMOTECONTROLLERAPI_SERVER_SERVER_LOOP_H
