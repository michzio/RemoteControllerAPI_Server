//
// Created by Michal Ziobro on 26/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_STREAM_SERVER_H
#define REMOTECONTROLLERAPI_SERVER_STREAM_SERVER_H

#include "../../networking/helpers/address_helper.h"
#include "generic_server.h"

// RPC TCP SERVER
result_t rpc_stream_server(server_info_t *info);

// EVENT TCP SERVER
result_t event_stream_server(server_info_t *info);

// ECHO TCP SERVER
result_t echo_stream_server(server_info_t *info); // only for testing
result_t echo_iterative_stream_server(server_info_t *info); // only for testing
result_t echo_concurrent_stream_server(server_info_t *info); // only for testing
result_t echo_managed_concurrent_stream_server(server_info_t *info); // only for testing
result_t echo_thread_pool_stream_server(server_info_t *info); // only for testing
result_t echo_pseudo_concurrent_stream_server(server_info_t *info); // only for testing

#endif //REMOTECONTROLLERAPI_SERVER_STREAM_SERVER_H
