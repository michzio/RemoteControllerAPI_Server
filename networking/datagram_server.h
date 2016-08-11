//
// Created by Michal Ziobro on 26/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_DATAGRAM_SERVER_H
#define REMOTECONTROLLERAPI_SERVER_DATAGRAM_SERVER_H

#include "../common/address_helper.h"

// ECHO UDP SERVER
result_t echo_datagram_server(void); // only for testing
result_t echo_iterative_datagram_server(void); // only for testing
result_t echo_concurrent_datagram_server(void); // only for testing
result_t echo_managed_concurrent_datagram_server(void); // only for testing
result_t echo_thread_pool_datagram_server(void); // only for testing

#endif //REMOTECONTROLLERAPI_SERVER_DATAGRAM_SERVER_H
