//
// Created by Michal Ziobro on 26/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_PASSIVE_CONNECTION_H
#define REMOTECONTROLLERAPI_SERVER_PASSIVE_CONNECTION_H

#include "helpers/address_helper.h"
#include "passive_socket.h"
#include "server_info.h"

#define BACKLOG 10  // number of pending connections that can be waiting on the listen queue on tcp socket

typedef result_t (*connection_handler_t)(server_info_t *server_info, sock_fd_t conn_sockfd);
typedef result_t (*request_handler_t)(sock_fd_t conn_sockfd);

result_t listen_connections(sock_fd_t ps_fd);
sock_fd_t accept_new_connection(sock_fd_t ps_fd);


#endif //REMOTECONTROLLERAPI_SERVER_PASSIVE_CONNECTION_H
