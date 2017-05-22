//
// Created by Michal Ziobro on 15/10/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_TEST_TRANSFER_H
#define REMOTECONTROLLERAPI_SERVER_TEST_TRANSFER_H

#include <memory.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "../../networking/common/network_types.h"
#include "../networking/generic_server.h"

typedef struct {
    void (*run_tests)(void);
} test_server_transfer_t;

extern test_server_transfer_t test_server_transfer;

#define TEST_PORT "3333"

// custom server loop handler - without looping, just single connection handling for testing purposes
static result_t test_stream_server_handler(sock_fd_t ps_fd, connection_handler_t handle_connection) {

    int cs_fd = accept_new_connection(ps_fd);
    if(cs_fd == FAILURE || cs_fd == CONTINUE) {
        fprintf(stderr, "accept_new_connection: failed!\n");
        return FAILURE;
    }

    printf("Handle connection on the main thread...\n");

    switch (handle_connection(cs_fd)) {
        case FAILURE:
            fprintf(stderr, "handle_connection: failed!\n");
            break;
        case CLOSED:
            printf("handle_connection: closed!\n");
            break;
        default:
            break;
    }

    if(close(cs_fd) < 0){
        fprintf(stderr, "close: %s\n", strerror(errno));
        return FAILURE;
    }

    return SUCCESS;
}

// generic method creating stream server with custom connection handler
static void test_create_stream_server(connection_handler_t test_conn_handler) {

    server_info_t *info;
    server_info_init(info);
    server_info_set_port(info, TEST_PORT);

    if(create_stream_server(info, test_stream_server_handler, test_conn_handler)) {
        fprintf(stderr, "create_stream_server: failed!\n");
        server_info_free(info);
        return;
    }

    server_info_free(info);
}

#endif //REMOTECONTROLLERAPI_SERVER_TEST_TRANSFER_H
