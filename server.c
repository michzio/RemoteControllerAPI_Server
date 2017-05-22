/*
 * Created by Michal Ziobro on 21/07/2016.
 */

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "server.h"
#include "networking/stream_server.h"
#include "networking/datagram_server.h"


int start_server(server_t server, server_info_t *server_info) {

    server(server_info);

    return SUCCESS;
}

int end_server(server_info_t *server_info) {

    sock_fd_t ps_fd = server_info_sock(server_info);

    if(close(ps_fd) < 0) {
        fprintf(stderr, "close: %s\n", strerror(errno));
        return FAILURE;
    }

    return SUCCESS;
}