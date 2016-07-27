/*
 * Created by Michal Ziobro on 21/07/2016.
 */

#include "server.h"
#include "networking/stream_server.h"
#include "networking/datagram_server.h"


int start_server(void) {

    // echo_stream_server();
    echo_datagram_server();

    return SUCCESS;
}

int end_server() {

    return SUCCESS;
}