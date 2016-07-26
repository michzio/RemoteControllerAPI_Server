//
// Created by Michal Ziobro on 26/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_GENERIC_SERVER_H
#define REMOTECONTROLLERAPI_SERVER_GENERIC_SERVER_H

#include "../common/address_helper.h"
#include "passive_connection.h"
#include "server_loop.h"

result_t create_stream_server(const char *port, stream_server_loop_t server_loop, connection_handler_t conn_handler);
result_t create_datagram_server(const char *port, datagram_server_loop_t server_loop, datagram_handler_t datagram_handler);

#endif //REMOTECONTROLLERAPI_SERVER_GENERIC_SERVER_H
