//
// Created by Michal Ziobro on 26/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_SERVICE_H
#define REMOTECONTROLLERAPI_SERVER_SERVICE_H

#include "../common/address_helper.h"
#include "passive_socket.h"

// TCP
result_t rpc_service_connection_handler(sock_fd_t sock_fd);
result_t event_service_connection_handler(sock_fd_t sock_fd);
result_t echo_service_connection_handler(sock_fd_t sock_fd); // only for testing
result_t echo_service_request_handler(sock_fd_t sock_fd); // only for testing

// UDP
result_t echo_service_datagram_handler(sock_fd_t sock_fd, const struct sockaddr *peer_addr, const char *datagram);

#endif //REMOTECONTROLLERAPI_SERVER_SERVICE_H
