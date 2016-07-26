//
// Created by Michal Ziobro on 25/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_PASSIVE_SOCKET_H
#define REMOTECONTROLLERAPI_SERVER_PASSIVE_SOCKET_H

#include "../common/address_helper.h"

typedef int sock_type_t;
typedef int sock_fd_t;

result_t create_stream_pasv_sock(const char *port, sock_fd_t *pasv_sock_fd);
result_t create_datagram_pasv_sock(const char *port, sock_fd_t *pasv_sock_fd);

#endif //REMOTECONTROLLERAPI_SERVER_PASSIVE_SOCKET_H
