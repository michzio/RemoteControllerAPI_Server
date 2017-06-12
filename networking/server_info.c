//
// Created by Michal Ziobro on 08/06/2017.
//

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include "server_info.h"
#include "datagram.h"

struct server_info {

    pthread_cond_t cond;
    pthread_mutex_t mutex;

    volatile int SHUT_DOWN_FLAG;
    volatile int FORCE_SHUT_DOWN_FLAG;

    char *port;                 // port passive socket binds
    char *ip;                   // ip address of server host
    sock_fd_t sockfd;           // server passive socket
    char *security_password;    // connection handlers can use it to authorize clients

    size_t conn_sockfds_len;
    doubly_linked_list_t *conn_sockfds; // list of active connection sockets

    // server events callbacks
    server_start_callback_t server_start_callback;
    server_end_callback_t server_end_callback;
    server_error_callback_t server_error_callback;
    client_connected_callback_t client_connected_callback;
    client_disconnecting_callback_t client_disconnecting_callback;
    connection_error_callback_t connection_error_callback;
    datagram_error_callback_t datagram_error_callback;

    // server events callbacks custom arguments
    void *server_start_callback_arg;
    void *server_end_callback_arg;
    void *server_error_callback_arg;
    void *client_connected_callback_arg;
    void *client_disconnecting_callback_arg;
    void *connection_error_callback_arg;
    void *datagram_error_callback_arg;
};

// server_info_t operations
void server_info_init(server_info_t **info) {

    *info = malloc(sizeof(server_info_t));
    (*info)->ip = NULL;
    (*info)->port = NULL;
    (*info)->security_password = NULL;

    allocator_t *allocator;
    allocator_init(&allocator, int_allocate_handler, int_deallocate_handler);

    dl_list_init( &((*info)->conn_sockfds), allocator);
    (*info)->conn_sockfds_len = 0;

    (*info)->server_start_callback = NULL;
    (*info)->server_end_callback = NULL;
    (*info)->server_error_callback = NULL;
    (*info)->client_connected_callback = NULL;
    (*info)->client_disconnecting_callback = NULL;
    (*info)->connection_error_callback = NULL;
    (*info)->datagram_error_callback = NULL;

    (*info)->server_start_callback_arg = NULL;
    (*info)->server_end_callback_arg = NULL;
    (*info)->server_error_callback_arg = NULL;
    (*info)->client_connected_callback_arg = NULL;
    (*info)->client_disconnecting_callback_arg = NULL;
    (*info)->connection_error_callback_arg = NULL;
    (*info)->datagram_error_callback_arg = NULL;

    (*info)->SHUT_DOWN_FLAG = 0;
    (*info)->FORCE_SHUT_DOWN_FLAG = 0;

    pthread_mutex_init( &((*info)->mutex), NULL);
    pthread_cond_init( &((*info)->mutex), NULL);
}

void server_info_set_shut_down(server_info_t *info, int flag) {

    info->SHUT_DOWN_FLAG = flag;
}

int server_info_should_shut_down(server_info_t *info) {

    return info->SHUT_DOWN_FLAG;
}

void server_info_set_force_shut_down(server_info_t *info, int flag) {

    info->FORCE_SHUT_DOWN_FLAG = flag;
}

int server_info_force_shut_down(server_info_t *info) {

    return info->FORCE_SHUT_DOWN_FLAG;
}

void server_info_set_port(server_info_t *info, const char *port) {

    if(info->port == NULL) {
        info->port = malloc(sizeof(port));
    } else {
        info->port = realloc(info->port, sizeof(port));
    }
    strcpy(info->port, port);
}

void server_info_set_ip(server_info_t *info, const char *ip) {

    if(info->ip == NULL) {
        info->ip = malloc(sizeof(ip));
    } else {
        info->ip = realloc(info->ip, sizeof(ip));
    }
    strcpy(info->ip, ip);
}

void server_info_set_sock(server_info_t *info, const sock_fd_t sockfd) {

    info->sockfd = sockfd;
}

void server_info_set_security_password(server_info_t *info, const char *password) {

    info->security_password = password;
}

result_t server_info_fill(server_info_t *info, const sock_fd_t sockfd) {

    info->sockfd = sockfd;

    char *ip_address;
    int port_number;

    if(get_current_address_and_port(sockfd, &ip_address, &port_number) == FAILURE) {
        fprintf(stderr, "get_current_address_and_port: faild!\n");
        return FAILURE;
    }

    // converting integer port number into string equivalent
    size_t port_len = (size_t)((ceil(log10(port_number))+1)*sizeof(char));
    char *port = malloc(port_len);
    snprintf(port, port_len, "%d", port_number);

    if(info->ip) free(info->ip);
    info->ip = ip_address;
    if(info->port) free(info->port);
    info->port = port;

    return SUCCESS;
}


const char *server_info_port(const server_info_t *info) {

    return info->port;
}

const char *server_info_ip(const server_info_t *info) {

    return info->ip;
}

const sock_fd_t server_info_sock(const server_info_t *info) {

    return info->sockfd;
}

const char *server_info_security_password(server_info_t *info) {

    return info->security_password;
}

// server_info_t storing connection sockets

void server_info_add_conn_sock(server_info_t *info, const sock_fd_t sockfd) {

    pthread_mutex_lock(&info->mutex);

    (info->conn_sockfds_len)++;
    dl_list_push_back(info->conn_sockfds, &sockfd, sizeof(sock_fd_t));

    printf("Adding connection socket: %d to server_info_t. Total number: %d\n", sockfd, info->conn_sockfds_len);

    pthread_mutex_unlock(&info->mutex);

}

result_t server_info_remove_conn_sock(server_info_t *info, const sock_fd_t sockfd) {


    pthread_mutex_lock(&info->mutex);

    doubly_linked_node_t *conn_sock_node = dl_list_find_first(info->conn_sockfds, &sockfd, int_ptr_cmp_func);
    result_t res = dl_list_remove_node(info->conn_sockfds, conn_sock_node);
    if(res == SUCCESS) {
        (info->conn_sockfds_len)--;
        printf("Removing connection socket: %d from server_info_t. Total number: %d\n", sockfd, info->conn_sockfds_len);
        pthread_cond_signal(&info->cond);
    }
    pthread_mutex_unlock(&info->mutex);

    return res;
}

const doubly_linked_list_t *server_info_conn_socks(server_info_t *info) {
    return info->conn_sockfds;
}

void server_info_shutdown_conn_socks(server_info_t *info) {

    pthread_mutex_lock(&info->mutex);
    doubly_linked_node_t *node = dl_list_front(info->conn_sockfds);
    pthread_mutex_unlock(&info->mutex);

    if(node != NULL) {
       do {

            pthread_mutex_lock(&info->mutex);

            sock_fd_t *conn_sock = (sock_fd_t *) dl_unwrap_data(node, NULL);

            printf("Connection sock: %d closing...\n", *conn_sock);
            if (shutdown(*conn_sock, SHUT_RDWR) < 0) {
                fprintf(stderr, "shutdown: failed! %s\n", strerror(errno));
                return;
            }

            node = dl_list_next(node);

            pthread_mutex_unlock(&info->mutex);
        } while (node != dl_list_front(info->conn_sockfds));
    }

}

// set event handlers (callbacks)
void server_info_set_server_start_callback(server_info_t *info, server_start_callback_t callback) {
    info->server_start_callback = callback;
}

void server_info_set_server_end_callback(server_info_t *info, server_end_callback_t callback) {
    info->server_end_callback = callback;
}

void server_info_set_server_error_callback(server_info_t *info, server_error_callback_t callback) {
    info->server_error_callback = callback;
}

void server_info_set_client_connected_callback(server_info_t *info, client_connected_callback_t callback) {
    info->client_connected_callback = callback;
}

void server_info_set_client_disconnecting_callback(server_info_t *info, client_disconnecting_callback_t callback) {
    info->client_disconnecting_callback = callback;
}
void server_info_set_connection_error_callback(server_info_t *info, connection_error_callback_t callback) {
    info->connection_error_callback = callback;
}

void server_info_set_datagram_error_callback(server_info_t *info, datagram_error_callback_t callback) {
    info->datagram_error_callback = callback;
}

// set callbacks custom arguments
void server_info_set_server_start_callback_arg(server_info_t *info, void *callback_arg) {
    info->server_start_callback_arg = callback_arg;
}
void server_info_set_server_end_callback_arg(server_info_t *info, void *callback_arg) {
    info->server_end_callback_arg = callback_arg;
}
void server_info_set_server_error_callback_arg(server_info_t *info, void *callback_arg) {
    info->server_error_callback_arg = callback_arg;
}
void server_info_set_client_connected_callback_arg(server_info_t *info, void *callback_arg) {
    info->client_connected_callback_arg = callback_arg;
}
void server_info_set_client_disconnecting_callback_arg(server_info_t *info, void *callback_arg) {
    info->client_disconnecting_callback_arg = callback_arg;
}
void server_info_set_connection_error_callback_arg(server_info_t *info, void *callback_arg) {
    info->connection_error_callback_arg = callback_arg;
}
void server_info_set_datagram_error_callback_arg(server_info_t *info, void *callback_arg) {
    info->datagram_error_callback_arg = callback_arg;
}

// get event handlers (callbacks)
server_start_callback_t server_info_server_start_callback(server_info_t *info) {
    return info->server_start_callback;
}

server_end_callback_t server_info_server_end_callback(server_info_t *info) {
    return info->server_end_callback;
}

server_error_callback_t server_info_server_error_callback(server_info_t *info) {
    return info->server_error_callback;
}

client_connected_callback_t server_info_client_connected_callback(server_info_t *info) {
    return info->client_connected_callback;
}

client_disconnecting_callback_t server_info_client_disconnecting_callback(server_info_t *info) {
    return info->client_disconnecting_callback;
}

connection_error_callback_t  server_info_connection_error_callback(server_info_t *info) {
    return info->connection_error_callback;
}

datagram_error_callback_t  server_info_datagram_error_callback(server_info_t *info) {
    return info->datagram_error_callback;
}

// get callbacks custom arguments
void *server_info_server_start_callback_arg(server_info_t *info) {
    return info->server_start_callback_arg;
}

void *server_info_server_end_callback_arg(server_info_t *info) {
    return info->server_end_callback_arg;
}

void *server_info_server_error_callback_arg(server_info_t *info) {
    return info->server_error_callback_arg;
}

void *server_info_client_connected_callback_arg(server_info_t *info) {
    return info->client_connected_callback_arg;
}

void *server_info_client_disconnecting_callback_arg(server_info_t *info) {
    return info->client_disconnecting_callback_arg;
}

void *server_info_connection_error_callback_arg(server_info_t *info) {
    return info->connection_error_callback_arg;
}

void *server_info_datagram_error_callback_arg(server_info_t *info) {
    return info->datagram_error_callback_arg;
}

// server events helper methods
void server_info_server_start_event(server_info_t *info) {

    char *server_ip;
    int server_port;

    get_current_address_and_port(info->sockfd, &server_ip, &server_port);

    if(info->server_start_callback != NULL)
        info->server_start_callback(info->sockfd, server_port, server_ip, info->server_start_callback_arg);
}

void server_info_server_end_event(server_info_t *info) {

    server_end_callback_t callback = info->server_end_callback;
    void *callback_arg = info->server_end_callback_arg;

    server_info_free(info);

    if( callback != NULL) callback(callback_arg);

}

void server_info_server_error_event(server_info_t *info, const int error_code, const char *error_msg) {

    if(info->server_error_callback != NULL)
        info->server_error_callback(info->sockfd, error_code, error_msg, info->server_error_callback_arg);
}

void server_info_client_connected_event(server_info_t *info, sock_fd_t conn_sockfd) {

    char *client_ip;
    int client_port;
    char *server_ip;
    int server_port;

    get_peer_address_and_port(conn_sockfd, &client_ip, &client_port);
    // printf("Client ip address: %s and port: %d\n", client_ip, client_port);
    get_current_address_and_port(conn_sockfd, &server_ip, &server_port);
    // printf("Server ip address: %s and port: %d\n", server_ip, server_port);

    server_info_set_ip(info, server_ip); // update server_info ip address to real server ip address used to connect clients
    server_info_add_conn_sock(info, conn_sockfd);

    if(info->client_connected_callback != NULL)
        info->client_connected_callback(conn_sockfd, client_port, client_ip, info->client_connected_callback_arg);
}

void server_info_client_disconnecting_event(server_info_t *info, sock_fd_t conn_sockfd) {

    if(server_info_force_shut_down(info)) {
        if(info->client_disconnecting_callback != NULL)
            info->client_disconnecting_callback(conn_sockfd, -1, NULL, info->client_disconnecting_callback_arg);

    } else {

        char *client_ip;
        int client_port;

        get_peer_address_and_port(conn_sockfd, &client_ip, &client_port);

        if (info->client_disconnecting_callback != NULL)
            info->client_disconnecting_callback(conn_sockfd, client_port, client_ip, info->client_disconnecting_callback_arg);
    }

    server_info_remove_conn_sock(info, conn_sockfd);
}

void server_info_connection_error_event(server_info_t *info, sock_fd_t conn_sockfd, const int error_code, const char *error_msg) {

    if(info->connection_error_callback != NULL)
        info->connection_error_callback(conn_sockfd, error_code, error_msg, info->connection_error_callback_arg);

    if(conn_sockfd >= 0) {
        server_info_remove_conn_sock(info, conn_sockfd);
    }
}

void server_info_datagram_error_event(server_info_t *info, const int error_code, const char *error_msg) {

    if(info->datagram_error_callback != NULL)
        info->datagram_error_callback(info->sockfd, error_code, error_msg, info->datagram_error_callback_arg);
}


void server_info_free(server_info_t *info) {

    pthread_mutex_lock(&info->mutex);
    while(info->conn_sockfds_len > 0)
        pthread_cond_wait(&info->cond, &info->mutex);

    dl_list_free(info->conn_sockfds); info->conn_sockfds = NULL;

    pthread_mutex_unlock(&info->mutex);

    if(info->port) free(info->port);
    if(info->ip) free(info->ip);

    // set callbacks to NULL
    info->server_start_callback = NULL;
    info->server_end_callback = NULL;
    info->server_error_callback = NULL;
    info->client_connected_callback = NULL;
    info->client_disconnecting_callback = NULL;
    info->connection_error_callback = NULL;
    info->datagram_error_callback = NULL;

    info->server_start_callback_arg = NULL;
    info->server_end_callback_arg = NULL;
    info->server_error_callback_arg = NULL;
    info->client_connected_callback_arg = NULL;
    info->client_disconnecting_callback_arg = NULL;
    info->connection_error_callback_arg = NULL;
    info->datagram_error_callback_arg = NULL;

    info->SHUT_DOWN_FLAG = 0;
    info->FORCE_SHUT_DOWN_FLAG = 0;

    pthread_mutex_destroy(&info->mutex);

    free(info); info = NULL;
}