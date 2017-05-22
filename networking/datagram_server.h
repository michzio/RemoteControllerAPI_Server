//
// Created by Michal Ziobro on 26/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_DATAGRAM_SERVER_H
#define REMOTECONTROLLERAPI_SERVER_DATAGRAM_SERVER_H

#include "../../networking/helpers/address_helper.h"
#include "generic_server.h"

// ECHO UDP SERVER
result_t echo_datagram_server(server_info_t *info); // only for testing
result_t echo_iterative_datagram_server(server_info_t *info); // only for testing
result_t echo_concurrent_datagram_server(server_info_t *info); // only for testing
result_t echo_managed_concurrent_datagram_server(server_info_t *info); // only for testing
result_t echo_thread_pool_datagram_server(server_info_t *info); // only for testing

#endif //REMOTECONTROLLERAPI_SERVER_DATAGRAM_SERVER_H
