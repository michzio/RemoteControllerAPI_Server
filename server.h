/*
 * Created by Michal Ziobro on 21/07/2016.
 */

#ifndef REMOTECONTROLLERAPI_SERVER_SERVER_H
#define REMOTECONTROLLERAPI_SERVER_SERVER_H

#include <netinet/in.h>
#include <netdb.h>
#include "../networking/helpers/address_helper.h"
#include "networking/generic_server.h"

result_t start_server(server_t server, server_info_t *server_info);
result_t end_server(server_info_t *server_info);
result_t shutdown_server(server_info_t *server_info);

#endif /* REMOTECONTROLLERAPI_SERVER_SERVER_H */
