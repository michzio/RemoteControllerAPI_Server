/*
 * Created by Michal Ziobro on 21/07/2016.
 */

#ifndef REMOTECONTROLLERAPI_SERVER_SERVER_H
#define REMOTECONTROLLERAPI_SERVER_SERVER_H

#include <netinet/in.h>
#include <netdb.h>
#include "helper.h"

#define PORT "2016" // the port remote controller server will be listening for connections
#define BACKLOG 10  // number of pending connections that can be waiting on the listen queue

int start_server(void);
result_t create_passive_socket(int *res_fd);
result_t listen_connections(int ps_fd);
int wait_for_connections(int ps_fd);
int accept_new_connection(int ps_fd);
int handle_communication(int cs_fd);
int end_server(void);
result_t echo(int cs_fd);

#endif /* REMOTECONTROLLERAPI_SERVER_SERVER_H */
