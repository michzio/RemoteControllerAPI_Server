#include <stdio.h>
#include "test/test_server_loop.h"
#include "server.h"
#include "test/test_transfer.h"
#include "networking/stream_server.h"

#define PORT "0" // the port remote controller server will be listening for connections

int main(int argc, char **argv) {

    printf("Remote Controller Server - desktop side, remotely controlled.\n");

    //test_server_transfer.run_tests();
    //test_server_loop.run_tests();

    char *ip_address;
    get_current_address(AF_INET, &ip_address);
    printf("Current device IP address: %s\n", ip_address);

    server_info_t *server_info;
    server_info_init(&server_info);
    server_info_set_port(server_info, PORT);
    //server_info_set_ip();
    //server_info_set_sock();
    //server_info_port();
    //server_info_ip();
    //server_info_sock();

    start_server(echo_iterative_stream_server, server_info);

    server_info_free(server_info);

    // start_server(echo_iterative_datagram_server, server_info);
    // start_server(echo_concurrent_stream_server, server_info);
    // start_server(echo_concurrent_datagram_server, server_info);
    // start_server(echo_managed_concurrent_stream_server, server_info);
    // start_server(echo_managed_concurrent_datagram_server, server_info);
    // start_server(echo_thread_pool_stream_server, server_info);
    // start_server(echo_thread_pool_datagram_server, server_info);
    // start_server(echo_pseudo_concurrent_stream_server, server_inf);

    return 0;
}