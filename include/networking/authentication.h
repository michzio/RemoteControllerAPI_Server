//
// Created by Michal Ziobro on 19/06/2017.
//

#include "types.h"
#include "common/network_types.h"
#include "server_info.h"

#ifndef REMOTECONTROLLERAPI_SERVER_AUTHENTICATION_H
#define REMOTECONTROLLERAPI_SERVER_AUTHENTICATION_H

result_t authenticate_client(const server_info_t *server_info, const sock_fd_t sock_fd, char *client_identity, char *client_os);

#endif //REMOTECONTROLLERAPI_SERVER_AUTHENTICATION_H
