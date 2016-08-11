//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include "stream_server.h"
#include "generic_server.h"
#include "../server.h"
#include "service.h"

// RPC TCP SERVER
result_t rpc_stream_server(void) {

    // TODO

    return SUCCESS;
}

// EVENT TCP SERVER
result_t event_stream_server(void) {

    // TODO

    return SUCCESS;
}

// ECHO TCP SERVER
result_t echo_stream_server(void) {
    return echo_iterative_stream_server();
}

result_t echo_iterative_stream_server(void) {

    if(create_stream_server(PORT, iterative_stream_server_loop, echo_service_connection_handler) == FAILURE) {
        fprintf(stderr, "create_stream_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

result_t echo_concurrent_stream_server(void) {

    if(create_stream_server(PORT, concurrent_stream_server_loop, echo_service_connection_handler) == FAILURE) {
        fprintf(stderr, "create_stream_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

result_t echo_managed_concurrent_stream_server(void) {

    if(create_stream_server(PORT, managed_concurrent_stream_server_loop, echo_service_connection_handler) == FAILURE) {
        fprintf(stderr, "create_stream_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

result_t echo_thread_pool_stream_server(void) {

    if(create_stream_server(PORT, thread_pool_stream_server_loop, echo_service_connection_handler) == FAILURE) {
        fprintf(stderr, "create_stream_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

result_t echo_pseudo_concurrent_stream_server(void) {

    if(create_stream_server(PORT, pseudo_concurrent_stream_server_loop, echo_service_connection_handler) == FAILURE) {
        fprintf(stderr, "create_stream_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}