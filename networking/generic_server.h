//
// Created by Michal Ziobro on 26/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_GENERIC_SERVER_H
#define REMOTECONTROLLERAPI_SERVER_GENERIC_SERVER_H

#include "../../networking/helpers/address_helper.h"
#include "passive_connection.h"
#include "server_loop.h"
#include "../../collections/linked_list/linked_list.h"
#include "server_info.h"

// server_t function implementing logic of specific server
typedef result_t (*server_t)(server_info_t *info);

// helper functions that helps to create server of given type (stream - TCP, datagram - UDP)
result_t create_stream_server(server_info_t *info, stream_server_loop_t server_loop, connection_handler_t conn_handler);
result_t create_datagram_server(server_info_t *info, datagram_server_loop_t server_loop, datagram_handler_t datagram_handler);

#endif //REMOTECONTROLLERAPI_SERVER_GENERIC_SERVER_H
