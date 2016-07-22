/*
 * Created by Michal Ziobro on 21/07/2016.
 */

#ifndef REMOTECONTROLLERAPI_SERVER_SERVER_H
#define REMOTECONTROLLERAPI_SERVER_SERVER_H

#include <netinet/in.h>
#include <netdb.h>

#define PORT "2016" // the port remote controller server will be listening for connections
#define BACKLOG 10  // number of pending connections that can be waiting on the listen queue

typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
} result_t;

typedef enum {
    PASSIVE_SOCKET = 0,
    CONNECTION_SOCKET = 1,
} socket_type_t;

int start_server(void);
int create_passive_socket(int *res_fd);
int listen_connections(int ps_fd);
int accept_new_connection(void);
int end_server(void);

int print_socket_address(int sockfd, socket_type_t socket_type);
int get_address_and_port_from_sockfd(int sockfd, char **ip_address, int *port);
int get_address_and_port_from_addrinfo(const struct addrinfo *addrinfo, char **ip_address, int *port);
int get_address_and_port_from_sockaddr(const struct sockaddr *sockaddr, char **ip_address, int *port);
void *get_in_addr(const struct sockaddr *sa);
in_port_t get_in_port(const struct sockaddr *sa);

#endif /* REMOTECONTROLLERAPI_SERVER_SERVER_H */
