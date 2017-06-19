//
// Created by Michal Ziobro on 19/06/2017.
//

#include <string.h>
#include <sys/socket.h>
#include <errno.h>
#include "authentication.h"
#include "../../common/strings.h"

#define MAX_BUF_SIZE 256

static result_t authentication_success(const server_info_t *server_info, const sock_fd_t sock_fd) {

    char buf[MAX_BUF_SIZE];
    int n_sent; // number of bytes sent

    strcpy(buf, "AUTHENTICATION SUCCESS");
    if((n_sent = send(sock_fd, buf, strlen(buf), 0)) < 0) {
        perror("send");
        // publish connection error event
        server_info_connection_error_event(server_info, sock_fd, CONN_ERROR_SEND, strerror(errno));
        return FAILURE;
    }

    return SUCCESS;
}

static void authentication_failed(const server_info_t *server_info, const sock_fd_t sock_fd) {

    char buf[MAX_BUF_SIZE];
    int n_sent; // number of bytes sent

    strcpy(buf, "AUTHENTICATION FAILED");
    if((n_sent = send(sock_fd, buf, strlen(buf), 0)) < 0) {
        perror("send");
        // publish connection error event
        server_info_connection_error_event(server_info, sock_fd, CONN_ERROR_SEND, strerror(errno));
    }
}

result_t authenticate_client(const server_info_t *server_info, const sock_fd_t sock_fd, char *client_identity) {

    char buf[MAX_BUF_SIZE];
    int n_recv; // number of bytes received
    int n_sent; // number of bytes sent

    printf("Client authentication ...\n");

    // authenticate client
    strcpy(buf, "AUTHENTICATE");
    if((n_sent = send(sock_fd, buf, strlen(buf), 0)) < 0) {
        perror("send");
        // publish connection error event
        server_info_connection_error_event(server_info, sock_fd, CONN_ERROR_SEND, strerror(errno));
        return FAILURE;
    }

    if((n_recv = recv(sock_fd, buf, sizeof(buf) - 1, 0)) <= 0) {
        perror("recv");
        // publish connection error event
        server_info_connection_error_event(server_info, sock_fd, CONN_ERROR_RECV, strerror(errno));
        return FAILURE;
    }
    buf[n_recv] = '\0';

    size_t num_tokens = 0;
    size_t num_subtokens = 0;
    char **tokens = str_split(buf, " PASS ", &num_tokens);
    char **subtokens = str_split(tokens[0], "ID ", &num_subtokens);

    if(num_tokens < 1 && num_subtokens < 2) {
        // minimal client authentication response is: ID <client_identity>
        // there is also possibility to provide password: ID <client_identity> PASS <security_password>
        fprintf(stderr, "Authentication protocol error. Incorrect client authentication response %s!\n", buf);
        // publish connection error event
        server_info_connection_error_event(server_info, sock_fd, CONN_ERROR_AUTH, "Authentication protocol error.");
        return FAILURE;
    }

    const char *required_password = server_info_security_password(server_info);
    if(required_password == NULL) {
        // get only the client identity
        printf("Client identity: %s\n", subtokens[1]);
        strcpy(client_identity, subtokens[1]);

        return authentication_success(server_info, sock_fd);
    } else {
        // check password and get client identity
        if(num_tokens == 2 && strcmp(tokens[1], required_password) == 0) {
            printf("Client identity: %s and password: %s\n", subtokens[1], tokens[1]);
            strcpy(client_identity, subtokens[1]);

            return authentication_success(server_info, sock_fd);
        } else {
            printf("Client provided password doesn't match required one.\n");

            authentication_failed(server_info, sock_fd);
            return FAILURE;
        }
    }
}