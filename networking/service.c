//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "service.h"
#include "../../unit_tests/common/terminal.h"

// TCP
result_t rpc_service_connection_handler(server_info_t *server_info, sock_fd_t sock_fd) {

    // TODO

    return SUCCESS;
}

result_t event_service_connection_handler(server_info_t *server_info, sock_fd_t sock_fd) {

    // TODO

    return SUCCESS;
}

#define MAX_BUF_SIZE 256

result_t echo_service_connection_handler(server_info_t *server_info, sock_fd_t sock_fd) {

    char buf[MAX_BUF_SIZE];
    int n_recv; // number of bytes received
    int n_sent; // number of bytes sent

    fcntl(sock_fd, F_SETFL, O_NONBLOCK);

    while(1) {

        if(server_info_should_shut_down(server_info))
            return CLOSED;

        if ((n_recv = recv(sock_fd, buf, sizeof(buf) - 1, 0)) <= 0) {
            if(n_recv == 0) {
                printf("echo connection is closing...\n");
                return CLOSED;
            }
            if( (errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                // call to recv() on non-blocking socket result with nothing to receive
                continue;
            }
            perror("recv");
            // publish connection error event
            server_info_connection_error_event(server_info, sock_fd, CONN_ERROR_RECV, strerror(errno));
            return FAILURE;
        }

        buf[n_recv] = '\0';

        printf(ANSI_COLOR_BLUE "server: received '%s'" ANSI_COLOR_RESET "\n", buf);

        if ((n_sent = send(sock_fd, buf, strlen(buf), 0)) < 0) {
            perror("send");
            // publish connection error event
            server_info_connection_error_event(server_info, sock_fd, CONN_ERROR_SEND, strerror(errno));
            return FAILURE;
        }
    }

    return SUCCESS;
}

/**
 * instead of handling connection with client infinitely
 * request handler handles only a single request from the client
 * this makes it useful in the case of pseudo concurrent servers
 * that need to handle multiple connections on single thread.
 * request handler should have the same prototype as connection handler
 */
result_t echo_service_request_handler(sock_fd_t sock_fd) {

    char buf[MAX_BUF_SIZE];
    int n_recv; // number of bytes received
    int n_sent; // number of bytes sent

    if ((n_recv = recv(sock_fd, buf, sizeof(buf) - 1, 0)) <= 0) {
        if(n_recv == 0) {
            printf("echo connection is closing...\n");
            return CLOSED;
        }
        perror("recv");
        return FAILURE;
    }

    buf[n_recv] = '\0';

    printf(ANSI_COLOR_BLUE "server: received '%s'" ANSI_COLOR_RESET "\n", buf);

    if ((n_sent = send(sock_fd, buf, strlen(buf), 0)) < 0) {
        perror("send");
        return FAILURE;
    }

    return SUCCESS;
}

// UDP
result_t echo_service_datagram_handler(sock_fd_t sock_fd, const struct sockaddr *peer_addr, const char *datagram) {

    int n_sent; // number of bytes sent

    printf(ANSI_COLOR_BLUE "server: received '%s'" ANSI_COLOR_RESET "\n", datagram);

    char *ip_address;
    int port;
    get_address_and_port_from_sockaddr(peer_addr, &ip_address, &port);
    printf("Sending datagram back to (%s, %d).\n", ip_address, port);

    if ((n_sent = sendto(sock_fd, datagram, strlen(datagram), 0, peer_addr, sizeof(*peer_addr))) < 0) {
        perror("send");
        return FAILURE;
    }

    return SUCCESS;
}