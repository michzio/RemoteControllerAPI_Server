//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "hash_map/hash_map.h"
#include "service_handler/keyboard_input.h"
#include "service_handler/mouse_input.h"
#include "service_handler/app_event.h"
#include "networking/service.h"
#include "common/terminal.h"
#include "networking/authentication.h"

#define MAX_BUF_SIZE 256

// TCP
result_t rpc_service_connection_handler(server_info_t *server_info, sock_fd_t sock_fd) {

    char buf[MAX_BUF_SIZE];
    int n_recv; // number of bytes received
    int n_sent; // number of bytes sent

    // authenticate client
    char client_identity[MAX_BUF_SIZE];
    char client_os[MAX_BUF_SIZE];
    if(authenticate_client(server_info, sock_fd, client_identity, client_os) == FAILURE) {
        fprintf(stderr, "authenticate_client: failed!\n");
        // publish connection error event
        server_info_connection_error_event(server_info, sock_fd, CONN_ERROR_AUTH, "authenticate_client: failed!");
        return FAILURE;
    }
    // publish client authenticated event
    server_info_client_authenticated_event(server_info, sock_fd, client_identity, client_os);

    fcntl(sock_fd, F_SETFL, O_NONBLOCK);

    while(1) {

        if (server_info_should_shut_down(server_info))
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

        // handle RPC request
        handle_rpc_request(buf, n_recv);
    }

    return SUCCESS;
}

static const char REQ_KEYBOARD_INPUT[] = "keyboard input";
static const char REQ_MOUSE_CLICK[] = "mouse click";
static const char REQ_MOUSE_DOUBLE_CLICK[] = "mouse double click";
static const char REQ_MOUSE_TRIPLE_CLICK[] = "mouse triple click";
static const char REQ_MOUSE_MOVE_BY[] = "mouse move by";
static const char REQ_MOUSE_MOVE_TO[] = "mouse move to";
static const char REQ_MOUSE_DRAG_BY[] = "mouse drag by";
static const char REQ_MOUSE_DRAG_TO[] = "mouse drag to";
static const char REQ_MOUSE_SCROLL[] = "mouse scroll";
static const char REQ_MOUSE_PINCH[] = "mouse pinch";
static const char REQ_MOUSE_TRIPLE_SWIPE[] = "mouse triple swipe";
static const char REQ_MOUSE_ROTATE[] = "mouse rotate";
static const char REQ_PLAYER[] = "player";
static const char REQ_BROWSER[] = "browser";
static const char REQ_SLIDE_SHOW[] = "slide show";
static const char REQ_SYSTEM[] = "system";
static const char REQ_SHORTCUT[] = "shortcut";

result_t handle_rpc_request(const char *request, size_t req_length) {

    printf("RPC request <%s>", request);

    if(strncmp(request, REQ_KEYBOARD_INPUT, sizeof(REQ_KEYBOARD_INPUT)-1 ) == 0) {
        handle_keyboard_input(request, req_length);
    } else if(strncmp(request, REQ_MOUSE_CLICK, sizeof(REQ_MOUSE_CLICK)-1) == 0) {
        handle_mouse_click(request, req_length);
    } else if(strncmp(request, REQ_MOUSE_MOVE_BY, sizeof(REQ_MOUSE_MOVE_BY)-1) == 0) {
        handle_mouse_move_by(request, req_length);
    } else if(strncmp(request, REQ_MOUSE_MOVE_TO, sizeof(REQ_MOUSE_MOVE_TO)-1) == 0) {
        handle_mouse_move_to(request, req_length);
    } else if(strncmp(request, REQ_MOUSE_DRAG_BY, sizeof(REQ_MOUSE_DRAG_BY)-1) == 0) {
        handle_mouse_drag_by(request, req_length);
    } else if(strncmp(request, REQ_MOUSE_DRAG_TO, sizeof(REQ_MOUSE_DRAG_TO)-1) == 0) {
        handle_mouse_drag_to(request, req_length);
    } else if(strncmp(request, REQ_MOUSE_DOUBLE_CLICK, sizeof(REQ_MOUSE_DOUBLE_CLICK)-1) == 0) {
        handle_mouse_double_click(request, req_length);
    } else if(strncmp(request, REQ_MOUSE_TRIPLE_CLICK, sizeof(REQ_MOUSE_TRIPLE_CLICK)-1) == 0) {
        handle_mouse_triple_click(request, req_length);
    } else if(strncmp(request, REQ_MOUSE_SCROLL, sizeof(REQ_MOUSE_SCROLL)-1) == 0) {
        handle_mouse_scroll(request, req_length);
    } else if(strncmp(request, REQ_MOUSE_PINCH, sizeof(REQ_MOUSE_PINCH)-1) == 0) {
        handle_mouse_pinch(request, req_length);
    } else if(strncmp(request, REQ_MOUSE_TRIPLE_SWIPE, sizeof(REQ_MOUSE_TRIPLE_SWIPE)-1) == 0) {
        handle_mouse_triple_swipe(request, req_length);
    } else if(strncmp(request, REQ_MOUSE_ROTATE, sizeof(REQ_MOUSE_ROTATE)-1) == 0) {
        handle_mouse_rotate(request, req_length);
    } else if(strncmp(request, REQ_PLAYER, sizeof(REQ_PLAYER) -1) == 0) {
        handle_player_event(request, req_length);
    } else if(strncmp(request, REQ_BROWSER, sizeof(REQ_BROWSER)-1) == 0) {
        handle_browser_event(request, req_length);
    } else if(strncmp(request, REQ_SLIDE_SHOW, sizeof(REQ_SLIDE_SHOW)-1) == 0) {
        handle_slide_show_event(request, req_length);
    } else if(strncmp(request, REQ_SYSTEM, sizeof(REQ_SYSTEM)-1) == 0) {
        handle_system_event(request, req_length);
    } else if(strncmp(request, REQ_SHORTCUT, sizeof(REQ_SHORTCUT)-1) == 0) {
        handle_shortcut_event(request, req_length);
    }
}

result_t event_service_connection_handler(server_info_t *server_info, sock_fd_t sock_fd) {

    // TODO

    return SUCCESS;
}

result_t echo_service_connection_handler(server_info_t *server_info, sock_fd_t sock_fd) {

    char buf[MAX_BUF_SIZE];
    int n_recv; // number of bytes received
    int n_sent; // number of bytes sent

    // authenticate client
    char client_identity[MAX_BUF_SIZE];
    char client_os[MAX_BUF_SIZE];
    if(authenticate_client(server_info, sock_fd, client_identity, client_os) == FAILURE) {
        fprintf(stderr, "authenticate_client: failed!\n");
        // publish connection error event
        server_info_connection_error_event(server_info, sock_fd, CONN_ERROR_AUTH, "authenticate_client: failed!");
        return FAILURE;
    }
    // publish client authenticated event
    server_info_client_authenticated_event(server_info, sock_fd, client_identity, client_os);

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