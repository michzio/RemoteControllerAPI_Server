//
// Created by Michal Ziobro on 25/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_PASSIVE_SOCKET_H
#define REMOTECONTROLLERAPI_SERVER_PASSIVE_SOCKET_H

#include "helpers/address_helper.h"
#include "common/network_types.h"

result_t create_stream_pasv_sock(const char *port, sock_fd_t *pasv_sock_fd);
result_t create_datagram_pasv_sock(const char *port, sock_fd_t *pasv_sock_fd);

#endif //REMOTECONTROLLERAPI_SERVER_PASSIVE_SOCKET_H
