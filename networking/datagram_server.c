//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include "datagram_server.h"
#include "generic_server.h"
#include "../server.h"
#include "service.h"

// ECHO UDP SERVER
result_t echo_datagram_server(void) {
    return echo_datagram_server();
}

result_t echo_iterative_datagram_server(void) {

    if(create_datagram_server(PORT, iterative_datagram_server_loop, echo_service_datagram_handler) == FAILURE) {
        fprintf(stderr, "create_datagram_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

result_t echo_concurrent_datagram_server(void) {

    if(create_datagram_server(PORT, concurrent_datagram_server_loop, echo_service_datagram_handler) == FAILURE) {
        fprintf(stderr, "create_datagram_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

result_t echo_managed_concurrent_datagram_server(void) {

    if(create_datagram_server(PORT, managed_concurrent_datagram_server_loop, echo_service_datagram_handler) == FAILURE) {
        fprintf(stderr, "create_datagram_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}

result_t echo_thread_pool_datagram_server(void) {

    if(create_datagram_server(PORT, thread_pool_datagram_server_loop, echo_service_datagram_handler) == FAILURE) {
        fprintf(stderr, "create_datagram_server: failed!\n");
        return FAILURE;
    }
    return SUCCESS;
}