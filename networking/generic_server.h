//
// Created by Michal Ziobro on 26/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_GENERIC_SERVER_H
#define REMOTECONTROLLERAPI_SERVER_GENERIC_SERVER_H

#include "../../networking/helpers/address_helper.h"
#include "passive_connection.h"
#include "server_loop.h"

// server_info_t contains information about created server
struct server_info;
typedef struct server_info server_info_t;

// server_info_t operations
void server_info_init(server_info_t **info);
void server_info_set_port(server_info_t *info, const char *port);
void server_info_set_ip(server_info_t *info, const char *ip);
void server_info_set_sock(server_info_t *info, const sock_fd_t sockfd);
result_t server_info_fill(server_info_t *info, const sock_fd_t sockfd);
const char *server_info_port(const server_info_t *info);
const char *server_info_ip(const server_info_t *info);
const sock_fd_t server_info_sock(const server_info_t *info);
void server_info_free(server_info_t *info);

// server_t function implementing logic of specific server
typedef result_t (*server_t)(server_info_t *info);

// helper functions that helps to create server of given type (stream - TCP, datagram - UDP)
result_t create_stream_server(server_info_t *info, stream_server_loop_t server_loop, connection_handler_t conn_handler);
result_t create_datagram_server(server_info_t *info, datagram_server_loop_t server_loop, datagram_handler_t datagram_handler);

#endif //REMOTECONTROLLERAPI_SERVER_GENERIC_SERVER_H
