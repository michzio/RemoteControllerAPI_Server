//
// Created by Michal Ziobro on 13/08/2016.
//

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "test_server_loop.h"
#include "networking/stream_server.h"
#include "system.h"
#include "server.h"
#include "common/time.h"
#include "test/assertion.h"
#include "common/terminal.h"
#include "networking/generic_server.h"
#include "networking/datagram_server.h"

static void test_echo_server(server_t server, sock_type_t sock_type, const char *message, const char *test_name) {

    char *response;
    int pid;

    server_info_t *server_info;
    server_info_init(&server_info);
    server_info_set_port(server_info, TEST_PORT);

    // create server process
    if( (pid = fork()) < 0 ) {
        fprintf(stderr, "fork: failed!\n");
        return;
    } else if( pid == 0 ) {
        // CHILD PROCESS

        server(server_info);
    }
    // PARENT PROCESS
    response = exec_cmd_f("printf '%s' | nc %s %s %s -w %d",
                          message, (sock_type == SOCK_DGRAM) ? "-u" : "", LOCALHOST, TEST_PORT, DEFAULT_S_TIMEOUT);

    printf(ANSI_COLOR_BLUE "client: got response '%s'" ANSI_COLOR_RESET "\n", response);

    // terminate server process
    kill(pid, SIGTERM);
    while(waitpid(pid, NULL, WNOHANG)<=0);

    assert_equal(message, response, str_cmp_func, test_name);

    free(response);
}

static void test_iterative_stream_server_loop(void) {

    test_echo_server(echo_iterative_stream_server, SOCK_STREAM, "Hello, iterative tcp server!", __func__);
}

static void test_concurrent_stream_server_loop(void) {

    test_echo_server(echo_concurrent_stream_server, SOCK_STREAM, "Hello, concurrent tcp server!", __func__);
}

static void test_managed_concurrent_stream_server_loop(void) {

    test_echo_server(echo_managed_concurrent_stream_server, SOCK_STREAM, "Hello, managed concurrent tcp server!", __func__);
}

static void test_thread_pool_stream_server_loop(void) {

    test_echo_server(echo_thread_pool_stream_server, SOCK_STREAM, "Hello, thread pool tcp server!", __func__);
}

static void test_pseudo_concurrent_stream_server_loop(void) {

    test_echo_server(echo_pseudo_concurrent_stream_server, SOCK_STREAM, "Hello, pseudo concurrent tcp server!", __func__);
}

static void test_iterative_datagram_server_loop(void) {

    test_echo_server(echo_iterative_datagram_server, SOCK_DGRAM, "Hello, iterative udp server!", __func__);
}

static void test_concurrent_datagram_server_loop(void) {

    test_echo_server(echo_concurrent_datagram_server, SOCK_DGRAM, "Hello, concurrent udp server!", __func__);
}

static void test_managed_concurrent_datagram_server_loop(void) {

    test_echo_server(echo_managed_concurrent_datagram_server, SOCK_DGRAM, "Hello, managed concurrent udp server!", __func__);
}

static void test_thread_pool_datagram_server_loop(void) {

    test_echo_server(echo_thread_pool_datagram_server, SOCK_DGRAM, "Hello, thread pool udp server!", __func__);
}

static void run_tests(void) {

    // TCP server loops
    test_iterative_stream_server_loop();
    test_concurrent_stream_server_loop();
    test_managed_concurrent_stream_server_loop();
    test_thread_pool_stream_server_loop();
    test_pseudo_concurrent_stream_server_loop();

    // UDP server loops
    test_iterative_datagram_server_loop();
    test_concurrent_datagram_server_loop();
    test_managed_concurrent_datagram_server_loop();
    test_thread_pool_datagram_server_loop();

}

test_server_loop_t test_server_loop = { .run_tests = run_tests };