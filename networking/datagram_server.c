//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include "datagram_server.h"
#include "generic_server.h"
#include "../server.h"
#include "service.h"

result_t echo_datagram_server(void) {

    if(create_datagram_server(PORT, iterative_datagram_server_loop, echo_service_datagram_handler) == FAILURE) {
        fprintf(stderr, "create_datagram_server: failed!\n");
        return FAILURE;
    }

    return SUCCESS;
}