/*
 * Created by Michal Ziobro on 21/07/2016.
 */

#include "server.h"
#include "networking/stream_server.h"
#include "networking/datagram_server.h"


int start_server(server_t server, server_info_t *server_info) {

    server(server_info);

    return SUCCESS;
}

int end_server(server_info_t *server_info) {

    return SUCCESS;
}