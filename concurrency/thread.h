//
// Created by Michal Ziobro on 27/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_THREAD_H
#define REMOTECONTROLLERAPI_SERVER_THREAD_H

#define _REENTRANT
#include <pthread.h>

#include "../common/address_helper.h"
#include "../networking/datagram.h"
#include "../networking/passive_connection.h"

result_t connection_thread(sock_fd_t conn_sock_fd, connection_handler_t conn_handler);

#endif //REMOTECONTROLLERAPI_SERVER_THREAD_H
