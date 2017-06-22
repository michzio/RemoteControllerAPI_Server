//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include "stream_server.h"
#include "generic_server.h"
#include "../server.h"
#include "service.h"

// RPC TCP SERVER
result_t rpc_stream_server(server_info_t *info) {

    // TODO

    return SUCCESS;
}

// EVENT TCP SERVER
result_t event_stream_server(server_info_t *info) {

    // TODO

    return SUCCESS;
}

// ECHO TCP SERVER
result_t echo_stream_server(server_info_t *info) {
    return echo_thread_pool_stream_server(info);
}

result_t echo_iterative_stream_server(server_info_t *info) {

    if(create_stream_server(info, iterative_stream_server_loop, echo_service_connection_handler) == FAILURE) {
        fprintf(stderr, "create_stream_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

result_t echo_concurrent_stream_server(server_info_t *info) {

    if(create_stream_server(info, concurrent_stream_server_loop, echo_service_connection_handler) == FAILURE) {
        fprintf(stderr, "create_stream_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

result_t echo_managed_concurrent_stream_server(server_info_t *info) {

    if(create_stream_server(info, managed_concurrent_stream_server_loop, echo_service_connection_handler) == FAILURE) {
        fprintf(stderr, "create_stream_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

result_t echo_thread_pool_stream_server(server_info_t *info) {

    if(create_stream_server(info, thread_pool_stream_server_loop, echo_service_connection_handler) == FAILURE) {
        fprintf(stderr, "create_stream_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

result_t echo_pseudo_concurrent_stream_server(server_info_t *info) {

    if(create_stream_server(info, pseudo_concurrent_stream_server_loop, echo_service_request_handler) == FAILURE) {
        fprintf(stderr, "create_stream_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}