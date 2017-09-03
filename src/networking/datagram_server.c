//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include "networking/datagram_server.h"
#include "networking/generic_server.h"
#include "server.h"
#include "networking/service.h"

// ECHO UDP SERVER
result_t echo_datagram_server(server_info_t *info) {
    return echo_iterative_datagram_server(info);
}

result_t echo_iterative_datagram_server(server_info_t *info) {

    if(create_datagram_server(info, iterative_datagram_server_loop, echo_service_datagram_handler) == FAILURE) {
        fprintf(stderr, "create_datagram_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

result_t echo_concurrent_datagram_server(server_info_t *info) {

    if(create_datagram_server(info, concurrent_datagram_server_loop, echo_service_datagram_handler) == FAILURE) {
        fprintf(stderr, "create_datagram_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

result_t echo_managed_concurrent_datagram_server(server_info_t *info) {

    if(create_datagram_server(info, managed_concurrent_datagram_server_loop, echo_service_datagram_handler) == FAILURE) {
        fprintf(stderr, "create_datagram_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

result_t echo_thread_pool_datagram_server(server_info_t *info) {

    if(create_datagram_server(info, thread_pool_datagram_server_loop, echo_service_datagram_handler) == FAILURE) {
        fprintf(stderr, "create_datagram_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}