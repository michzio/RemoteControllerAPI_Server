/*
 * Created by Michal Ziobro on 21/07/2016.
 */

#ifndef REMOTECONTROLLERAPI_SERVER_SERVER_H
#define REMOTECONTROLLERAPI_SERVER_SERVER_H

#include <netinet/in.h>
#include <netdb.h>
#include "common/address_helper.h"

#define PORT "2016" // the port remote controller server will be listening for connections

int start_server(void);
int end_server(void);

#endif /* REMOTECONTROLLERAPI_SERVER_SERVER_H */
