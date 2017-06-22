#include <stdio.h>
#include "test/test_server_loop.h"
#include "server.h"
#include "test/test_transfer.h"
#include "networking/stream_server.h"
#include "../concurrency/threads/generic_thread.h"
#include "networking/datagram_server.h"

#define PORT "0" // the port remote controller server will be listening for connections

void on_server_start(const sock_fd_t ps_fd, const int server_port, const char *server_ip, void *callback_arg);
void on_server_end(const sock_fd_t ps_fd, void *callback_arg);
void on_server_error(const sock_fd_t ps_fd, const int error_code, const char *error_msg, void *callback_arg);
void on_client_connected(const sock_fd_t cs_fd, const int client_port, const char *client_ip, void *callback_arg);
void on_client_authenticated(const sock_fd_t cs_fd, const int client_port, const char *client_ip, const char *client_name, const char *client_os, void *callback_arg);
void on_client_disconnecting(const sock_fd_t cs_fd, const int client_port, const char *client_ip, void *callback_arg);
void on_connection_error(const sock_fd_t cs_fd, const int error_code, const char *error_msg, void *callback_arg);
void on_datagram_error(const sock_fd_t ps_fd, const int error_code, const char *error_msg, void *callback_arg);

runner_res_t background_thread_runner(void *arg) {

    start_server(echo_iterative_stream_server, (server_info_t *) arg);
}

int main(int argc, char **argv) {

    printf("Remote Controller Server - desktop side, remotely controlled.\n");

    //test_server_transfer.run_tests();
    //test_server_loop.run_tests();

    char *ip_address;
    get_current_address(AF_INET, &ip_address);
    printf("Current device IP address: %s\n", ip_address);

    char *callback_arg = "Some callback argument!";

    server_info_t *server_info;
    server_info_init(&server_info);
    server_info_set_port(server_info, PORT);
    server_info_set_security_password(server_info, "PaSSword12!");
    server_info_set_server_start_callback(server_info, on_server_start);
    server_info_set_server_start_callback_arg(server_info, callback_arg);
    server_info_set_server_end_callback(server_info, on_server_end);
    server_info_set_server_end_callback_arg(server_info, callback_arg); 
    server_info_set_server_error_callback(server_info, on_server_error);
    // only for TCP servers
    server_info_set_client_connected_callback(server_info, on_client_connected);
    server_info_set_client_authenticated_callback(server_info, on_client_authenticated);
    server_info_set_client_disconnecting_callback(server_info, on_client_disconnecting);
    server_info_set_connection_error_callback(server_info, on_connection_error);
    // only for UDP servers
    server_info_set_datagram_error_callback(server_info, on_datagram_error);
    //server_info_set_ip();
    //server_info_set_sock();
    //server_info_port();
    //server_info_ip();
    //server_info_sock();

    pthread_t thread = thread_create(background_thread_runner, server_info);

    sleep(30);

    end_server(server_info);
    //shutdown_server(server_info);

    /*
    sleep(30);

    server_info_t *server_info2;
    server_info_init(&server_info2);
    server_info_set_port(server_info2, PORT);

    pthread_t thread2 = thread_create(background_thread_runner, server_info2);

    sleep(10);
     */

    // start_server(echo_iterative_datagram_server, server_info);
    // start_server(echo_concurrent_stream_server, server_info);
    // start_server(echo_concurrent_datagram_server, server_info);
    // start_server(echo_managed_concurrent_stream_server, server_info);
    // start_server(echo_managed_concurrent_datagram_server, server_info);
    // start_server(echo_thread_pool_stream_server, server_info);
    // start_server(echo_thread_pool_datagram_server, server_info);
    // start_server(echo_pseudo_concurrent_stream_server, server_info);

    return 0;
}

void on_server_start(const sock_fd_t ps_fd, const int server_port, const char *server_ip, void *callback_arg) {

    printf("Server started bound to socket: %d (port number: %d, host name: %s)\n", ps_fd, server_port, server_ip);
    printf("on_server_start: user provided callback argument: %s\n", ((char *) callback_arg ) );
}

void on_server_end(const sock_fd_t ps_fd, void *callback_arg) {

    printf("Server with socket: %d ended!\n", ps_fd);
    printf("on_server_end: user provided callback argument: %s\n", ((char *) callback_arg ) );
}

void on_server_error(const sock_fd_t ps_fd, const int error_code, const char *error_msg, void *callback_arg) {

    printf("Server error: %d, message: %s\n", error_code, error_msg);
}

void on_client_connected(const sock_fd_t cs_fd, const int client_port, const char *client_ip, void *callback_arg) {

    printf("New client connected on conn socket: %d (port number: %d, host name: %s)\n", cs_fd, client_port, client_ip);
}

void on_client_authenticated(const sock_fd_t cs_fd, const int client_port, const char *client_ip, const char *client_name, const char *client_os, void *callback_arg) {

    printf("Client authenticated on conn socket: %d (port number: %d, host name: %s) with name: %s and os: %s\n",
            cs_fd, client_port, client_ip, client_name, client_os);
}

void on_client_disconnecting(const sock_fd_t cs_fd, const int client_port, const char *client_ip, void *callback_arg) {

    printf("Client disconnecting on conn socket: %d (port number: %d, host name: %s)\n", cs_fd, client_port, client_ip);
}

void on_connection_error(const sock_fd_t cs_fd, const int error_code, const char *error_msg, void *callback_arg) {

    printf("Error on conn socket: %d, error code: %d, error message: %s\n", cs_fd, error_code, error_msg);
}

void on_datagram_error(const sock_fd_t ps_fd, const int error_code, const char *error_msg, void *callback_arg) {

    printf("Error on datagram server socket: %d, error code: %d, error message: %s\n", ps_fd, error_code, error_msg);
}