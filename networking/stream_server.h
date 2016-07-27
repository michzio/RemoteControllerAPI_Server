//
// Created by Michal Ziobro on 26/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_STREAM_SERVER_H
#define REMOTECONTROLLERAPI_SERVER_STREAM_SERVER_H

#include "../common/address_helper.h"


result_t rpc_stream_server(void);
result_t event_stream_server(void);

result_t echo_stream_server(void); // only for testing
result_t echo_concurrent_stream_server(void); // only for testing

#endif //REMOTECONTROLLERAPI_SERVER_STREAM_SERVER_H
