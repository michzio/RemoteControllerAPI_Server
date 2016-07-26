//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include "stream_server.h"
#include "generic_server.h"
#include "../server.h"
#include "service.h"

result_t rpc_stream_server(void) {

    // TODO

    return SUCCESS;
}

result_t event_stream_server(void) {

    // TODO

    return SUCCESS;
}

result_t echo_stream_server(void) {

    if(create_stream_server(PORT, iterative_stream_server_loop, echo_service_handler) == FAILURE) {
        fprintf(stderr, "create_stream_server: failed!\n");
        return FAILURE;
    }

    return SUCCESS;
}