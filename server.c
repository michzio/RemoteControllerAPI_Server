/*
 * Created by Michal Ziobro on 21/07/2016.
 */

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "server.h"
#include "networking/stream_server.h"
#include "networking/datagram_server.h"


result_t start_server(server_t server, server_info_t *server_info) {

    return server(server_info);
}

result_t end_server(server_info_t *server_info) {

    server_info_set_shut_down(server_info, 1);

    return SUCCESS;
}

result_t shutdown_server(server_info_t *server_info) {

    server_info_set_force_shut_down(server_info, 1);
    server_info_shutdown_conn_socks(server_info);

    return SUCCESS;
}