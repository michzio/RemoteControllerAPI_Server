//
// Created by Michal Ziobro on 08/06/2017.
//

#ifndef REMOTECONTROLLERAPI_SERVER_SERVER_INFO_H
#define REMOTECONTROLLERAPI_SERVER_SERVER_INFO_H

#include "../../collections/linked_list/doubly_linked_list.h"
#include "../../networking/common/network_types.h"

// server_info_t contains information about created server
struct server_info;
typedef struct server_info server_info_t;

// server events callbacks
typedef void (*server_start_callback_t)(const sock_fd_t ps_fd, int server_port, const char *server_ip, void *callback_arg);
typedef void (*server_end_callback_t)(const sock_fd_t ps_fd, void *callback_arg);
typedef void (*server_error_callback_t)(const sock_fd_t ps_fd, const int error_code, const char *error_msg, void *callback_arg);
typedef void (*client_connected_callback_t)(const sock_fd_t cs_fd, const int client_port, const char *client_ip, void *callback_arg);
typedef void (*client_authenticated_callback_t)(const sock_fd_t cs_fd, const int client_port, const char *client_ip, const char *client_identity, const char *client_os, void *callback_arg);
typedef void (*client_disconnecting_callback_t)(const sock_fd_t cs_fd, const int client_port, const char *client_ip, void *callback_arg);
typedef void (*connection_error_callback_t)(const sock_fd_t cs_fd, const int error_code, const char *error_msg, void *callback_arg);
typedef void (*datagram_error_callback_t)(const sock_fd_t ps_fd, const int error_code, const char *error_msg, void *callback_arg);

// server_info_t operations
void server_info_init(server_info_t **info);

void server_info_set_shut_down(server_info_t *info, int flag);
int server_info_should_shut_down(server_info_t *info);

void server_info_set_force_shut_down(server_info_t *info, int flag);
int server_info_force_shut_down(server_info_t *info);

void server_info_set_port(server_info_t *info, const char *port);
void server_info_set_ip(server_info_t *info, const char *ip);
void server_info_set_sock(server_info_t *info, const sock_fd_t sockfd);
void server_info_set_security_password(server_info_t *info, const char *password);
result_t server_info_fill(server_info_t *info, const sock_fd_t sockfd);

const char *server_info_port(const server_info_t *info);
const char *server_info_ip(const server_info_t *info);
const sock_fd_t server_info_sock(const server_info_t *info);
const char *server_info_security_password(server_info_t *info);

// server_info_t storing connection sockets
void server_info_add_conn_sock(server_info_t *info, const sock_fd_t sockfd);
result_t server_info_remove_conn_sock(server_info_t *info, const sock_fd_t sockfd);
const doubly_linked_list_t *server_info_conn_socks(server_info_t *info);
void server_info_shutdown_conn_socks(server_info_t *info);

// set event handlers (callbacks)
void server_info_set_server_start_callback(server_info_t *info, server_start_callback_t callback);
void server_info_set_server_end_callback(server_info_t *info, server_end_callback_t callback);
void server_info_set_server_error_callback(server_info_t *info, server_error_callback_t callback);
void server_info_set_client_connected_callback(server_info_t *info, client_connected_callback_t callback);
void server_info_set_client_authenticated_callback(server_info_t *info, client_authenticated_callback_t callback);
void server_info_set_client_disconnecting_callback(server_info_t *info, client_disconnecting_callback_t callback);
void server_info_set_connection_error_callback(server_info_t *info, connection_error_callback_t callback);
void server_info_set_datagram_error_callback(server_info_t *info, datagram_error_callback_t callback);

// set callbacks custom arguments
void server_info_set_server_start_callback_arg(server_info_t *info, void *callback_arg);
void server_info_set_server_end_callback_arg(server_info_t *info, void *callback_arg);
void server_info_set_server_error_callback_arg(server_info_t *info, void *callback_arg);
void server_info_set_client_connected_callback_arg(server_info_t *info, void *callback_arg);
void server_info_set_client_authenticated_callback_arg(server_info_t *info, void *callback_arg);
void server_info_set_client_disconnecting_callback_arg(server_info_t *info, void *callback_arg);
void server_info_set_connection_error_callback_arg(server_info_t *info, void *callback_arg);
void server_info_set_datagram_error_callback_arg(server_info_t *info, void *callback_arg);

// get event handlers (callbacks)
server_start_callback_t server_info_server_start_callback(server_info_t *info);
server_end_callback_t server_info_server_end_callback(server_info_t *info);
server_error_callback_t server_info_server_error_callback(server_info_t *info);
client_connected_callback_t server_info_client_connected_callback(server_info_t *info);
client_authenticated_callback_t server_info_client_authenticated_callback(server_info_t *info);
client_disconnecting_callback_t server_info_client_disconnecting_callback(server_info_t *info);
connection_error_callback_t  server_info_connection_error_callback(server_info_t *info);
datagram_error_callback_t  server_info_datagram_error_callback(server_info_t *info);

// get callbacks custom arguments
void *server_info_server_start_callback_arg(server_info_t *info);
void *server_info_server_end_callback_arg(server_info_t *info);
void *server_info_server_error_callback_arg(server_info_t *info);
void *server_info_client_connected_callback_arg(server_info_t *info);
void *server_info_client_authenticated_callback_arg(server_info_t *info);
void *server_info_client_disconnecting_callback_arg(server_info_t *info);
void *server_info_connection_error_callback_arg(server_info_t *info);
void *server_info_datagram_error_callback_arg(server_info_t *info);

// helper methods that publish server events
void server_info_server_start_event(server_info_t *info);
void server_info_server_end_event(server_info_t *info);
void server_info_server_error_event(server_info_t *info, const int error_code, const char *error_msg);
void server_info_client_connected_event(server_info_t *info, sock_fd_t conn_sockfd);
void server_info_client_authenticated_event(server_info_t *info, sock_fd_t conn_sockfd, const char *client_identity, const char *client_os);
void server_info_client_disconnecting_event(server_info_t *info, sock_fd_t conn_sockfd);
void server_info_connection_error_event(server_info_t *info, sock_fd_t conn_sockfd, const int error_code, const char *error_msg);
void server_info_datagram_error_event(server_info_t *info, const int error_code, const char *error_msg);

void server_info_free(server_info_t *info);

// SERVER ERROR CONSTANTS
#define SERVER_ERROR_BASE 1000
#define SERVER_ERROR_CREATE_PASV_SOCK (SERVER_ERROR_BASE+1)
#define SERVER_ERROR_PRINT_ADDRESS (SERVER_ERROR_BASE+2)
#define SERVER_ERROR_INFO_FILL (SERVER_ERROR_BASE+3)
#define SERVER_ERROR_LISTEN_CONN (SERVER_ERROR_BASE+4)
#define SERVER_ERROR_LOOP (SERVER_ERROR_BASE+5)
#define SERVER_ERROR_CLOSE (SERVER_ERROR_BASE+6)
#define SERVER_ERROR_SELECT (SERVER_ERROR_BASE+7)

// CONNECTION ERROR CONSTANTS
#define CONN_ERROR_BASE 2000
#define CONN_ERROR_ACCEPT (CONN_ERROR_BASE+1)
#define CONN_ERROR_HANDLER (CONN_ERROR_BASE+2)
#define CONN_ERROR_CLOSE (CONN_ERROR_BASE+3)
#define CONN_ERROR_THREAD (CONN_ERROR_BASE+4)
#define CONN_ERROR_RECV (CONN_ERROR_BASE+5)
#define CONN_ERROR_SEND (CONN_ERROR_BASE+6)
#define CONN_ERROR_AUTH (CONN_ERROR_BASE+7)

// DATAGRAM ERROR CONSTANTS
#define DATAGRAM_ERROR_BASE 3000
#define DATAGRAM_ERROR_RECVFROM (DATAGRAM_ERROR_BASE+1)
#define DATAGRAM_ERROR_HANDLER (DATAGRAM_ERROR_BASE+2)
#define DATAGRAM_ERROR_THREAD (DATAGRAM_ERROR_BASE+3)

#endif //REMOTECONTROLLERAPI_SERVER_SERVER_INFO_H
