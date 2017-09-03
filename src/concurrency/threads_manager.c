//
// Created by Michal Ziobro on 08/08/2016.
//

#include <stdlib.h>
#include "concurrency/threads_manager.h"
#include "concurrency/runner.h"

/**
 * next functions check whether new thread can be created and max number of threads hasn't been exceeded.
 * they use threads manager object to make sure upper limit of number of threads hasn't been reached.
 * if number of threads has been exceeded than they wait until some current thread terminates and signals this
 */
result_t wait_for_connection_thread(threads_manager_t *threads_manager, connection_handler_t conn_handler, server_info_t *server_info, sock_fd_t conn_sock_fd) {

    conn_thread_runner_attr_t *conn_thread_runner_attr;
    conn_thread_runner_attr_init(&conn_thread_runner_attr);
    conn_thread_runner_attr_fill(conn_thread_runner_attr, conn_handler, server_info, conn_sock_fd, NULL, NULL);

    return wait_for_thread(threads_manager, (runner_t) connection_thread_runner, (runner_attr_t) conn_thread_runner_attr);
}
result_t wait_for_datagram_thread(threads_manager_t *threads_manager, datagram_handler_t datagram_handler, sock_fd_t sock_fd, struct sockaddr *peer_addr, char *datagram) {

    datagram_thread_runner_attr_t *datagram_thread_runner_attr;
    datagram_thread_runner_attr_init(&datagram_thread_runner_attr);
    datagram_thread_runner_attr_fill(datagram_thread_runner_attr, datagram_handler, sock_fd, peer_addr, datagram, NULL, NULL);

    return wait_for_thread(threads_manager, (runner_t) datagram_thread_runner, (runner_attr_t) datagram_thread_runner_attr);
}

result_t timed_wait_for_connection_thread(threads_manager_t *threads_manager, const int ms_timeout, connection_handler_t conn_handler, server_info_t *server_info, sock_fd_t conn_sock_fd) {

    conn_thread_runner_attr_t *conn_thread_runner_attr;
    conn_thread_runner_attr_init(&conn_thread_runner_attr);
    conn_thread_runner_attr_fill(conn_thread_runner_attr, conn_handler, server_info, conn_sock_fd, NULL, NULL);

    return timed_wait_for_thread(threads_manager, ms_timeout, (runner_t) connection_thread_runner, (runner_attr_t) conn_thread_runner_attr);
}

result_t timed_wait_for_datagram_thread(threads_manager_t *threads_manager, const int ms_timeout, datagram_handler_t datagram_handler, sock_fd_t sock_fd, struct sockaddr *peer_addr, char *datagram) {

    datagram_thread_runner_attr_t *datagram_thread_runner_attr;
    datagram_thread_runner_attr_init(&datagram_thread_runner_attr);
    datagram_thread_runner_attr_fill(datagram_thread_runner_attr, datagram_handler, sock_fd, peer_addr, datagram, NULL, NULL);

    return timed_wait_for_thread(threads_manager, ms_timeout, (runner_t) datagram_thread_runner, (runner_attr_t) datagram_thread_runner_attr);
}