//
// Created by Michal Ziobro on 26/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_DATAGRAM_H
#define REMOTECONTROLLERAPI_SERVER_DATAGRAM_H

#include "helpers/address_helper.h"
#include "passive_socket.h"

typedef  result_t (*datagram_handler_t) (sock_fd_t, const struct sockaddr *, const char *);

#endif //REMOTECONTROLLERAPI_SERVER_DATAGRAM_H
