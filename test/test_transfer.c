//
// Created by Michal Ziobro on 15/10/2016.
//

#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <errno.h>
#include "test_transfer.h"
#include "../../common/types.h"
#include "../../networking/common/network_types.h"
#include "../networking/generic_server.h"
#include "../../networking/transfer/integer_transfer.h"
#include "../../unit_tests/test/assertion.h"
#include "../../unit_tests/common/terminal.h"

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
}

// generic method creating stream server with custom connection handler
static void test_create_stream_server(connection_handler_t test_conn_handler) {

    if(create_stream_server(TEST_PORT, test_stream_server_handler, test_conn_handler)) {
        fprintf(stderr, "create_stream_server: failed!\n");
        return;
    }
}

static result_t uint8_transfer_handler(sock_fd_t cs_fd) {

    result_t res = 0;
    uint8_t recvedUint8 = 0;

    res = recv_uint8(cs_fd, &recvedUint8);
    assert_equal_int(res, SUCCESS, "Received uint8 number from socket");

    printf(ANSI_COLOR_CYAN "uint8 received: %d\n" ANSI_COLOR_RESET, recvedUint8);

    res = send_uint8(cs_fd, recvedUint8);
    assert_equal_int(res, SUCCESS, "Sent uint8 number to socket");

    return CLOSED;
}

static void test_uint8_transfer(void) {
    test_create_stream_server(uint8_transfer_handler);
}

static void run_tests(void) {

    printf( ANSI_COLOR_BLUE "Integration Test - requires to run: 'client' program only with 'test_transfer.run_tests()' \n" ANSI_COLOR_RESET);
    test_uint8_transfer();
}

test_server_transfer_t test_server_transfer = { .run_tests = run_tests };