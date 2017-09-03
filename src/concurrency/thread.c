//
// Created by Michal Ziobro on 27/07/2016.
//

#include <pthread.h>
#include "concurrency/thread.h"
#include "concurrency/runner.h"
#include "threads/generic_thread.h"

// create thread: connection thread, datagram thread

/**
 * @conn_handler - implements protocol used during communication between client and server
 * @server_info - stores information about server handling this connection
 * @conn_sock_fd - socket file descriptor used to communicate with connected client
 * @callback - optional callback function executed at the end of communication with results of conn_handler
 */
pthread_t connection_thread(connection_handler_t conn_handler, server_info_t *server_info, sock_fd_t conn_sock_fd) {
    return connection_thread_with_callback(conn_handler, server_info, conn_sock_fd, NULL, NULL);
}
pthread_t connection_thread_with_callback(connection_handler_t conn_handler, server_info_t *server_info, sock_fd_t conn_sock_fd, callback_t callback, callback_attr_t callback_attr) {

    conn_thread_runner_attr_t *conn_thread_runner_attr;
    conn_thread_runner_attr_init(&conn_thread_runner_attr);
    conn_thread_runner_attr_fill(conn_thread_runner_attr, conn_handler, server_info, conn_sock_fd, callback, callback_attr);

    return thread_create((runner_t) connection_thread_runner, (runner_attr_t) conn_thread_runner_attr);
}
/**
 * @datagram_handler - implements protocol used to handle datagram request from client and send him datagram response
 * @sock_fd - socket file descriptor used to send response back to the client
 * @peer_addr - peer address, where to send response back
 * @datagram - request datagram
 * @callback - optional callback function executed after datagram handling with results of datagram_handler
 */
pthread_t datagram_thread(datagram_handler_t datagram_handler, sock_fd_t sock_fd, struct sockaddr *peer_addr, char *datagram) {
    return datagram_thread_with_callback(datagram_handler, sock_fd, peer_addr, datagram, NULL, NULL);
}
pthread_t datagram_thread_with_callback(datagram_handler_t datagram_handler, sock_fd_t sock_fd, struct sockaddr *peer_addr, char *datagram, callback_t callback, callback_attr_t callback_attr) {

    datagram_thread_runner_attr_t *datagram_thread_runner_attr;
    datagram_thread_runner_attr_init(&datagram_thread_runner_attr);
    datagram_thread_runner_attr_fill(datagram_thread_runner_attr, datagram_handler, sock_fd, peer_addr, datagram, callback, callback_attr);

    return thread_create((runner_t) datagram_thread_runner, (runner_attr_t) datagram_thread_runner_attr);
}