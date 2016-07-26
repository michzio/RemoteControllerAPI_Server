//
// Created by Michal Ziobro on 24/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_ADDRESS_HELPER_H
#define REMOTECONTROLLERAPI_SERVER_ADDRESS_HELPER_H

#include <netdb.h>

typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
    CONTINUE = -2,
} result_t;

typedef enum {
    PASSIVE_SOCKET = 0,
    CONNECTION_SOCKET = 1,
} socket_type_t;

result_t print_socket_address(int sockfd, socket_type_t socket_type);
result_t get_current_address_and_port(int sockfd, char **ip_address, int *port);
result_t get_peer_address_and_port(int sockfd, char **ip_address, int *port);
result_t get_address_and_port_from_addrinfo(const struct addrinfo *addrinfo, char **ip_address, int *port);
result_t get_address_and_port_from_sockaddr(const struct sockaddr *sockaddr, char **ip_address, int *port);
void *get_in_addr(const struct sockaddr *sa);
in_port_t get_in_port(const struct sockaddr *sa);

#endif //REMOTECONTROLLERAPI_SERVER_ADDRESS_HELPER_H
