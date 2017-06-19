//
// Created by Michal Ziobro on 26/07/2016.
//

#include <sys/select.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "server_loop.h"
#include "../concurrency/thread.h"
#include "../concurrency/threads_manager.h"
#include "../concurrency/runner.h"
#include "../../concurrency/threads/thread_pool/thread_pool.h"

// TCP

/**
 * Function is looping infinitely and waiting
 * for new incoming client connections.
 * It handles connections one by one on the same thread.
 */
result_t iterative_stream_server_loop(server_info_t *server_info, connection_handler_t handle_connection) {

    sock_fd_t cs_fd, ps_fd;

    // get passive server socket
    ps_fd = server_info_sock(server_info);

    while(1) {

        if(server_info_should_shut_down(server_info)) {
            return CLOSED;
        }
        if(server_info_force_shut_down(server_info)) {
            return FORCE_CLOSED;
        }

        // check to accept new connection on the main thread...
        cs_fd = accept_new_connection(ps_fd);

        if(cs_fd == FAILURE) {
            fprintf(stderr, "accept_new_connection: failed!\n");
            server_info_connection_error_event(server_info, cs_fd, CONN_ERROR_ACCEPT, "accept_new_connection: failed!");
            return FAILURE;
        } else if(cs_fd == CONTINUE) {
            continue;
        }

        // publish client connected event
        server_info_client_connected_event(server_info, cs_fd);

        printf("Handle connection on the main thread...\n");

        // revert connection socket to non-blocking
        int opts = fcntl(cs_fd, F_GETFL);
        opts = opts & (~O_NONBLOCK);
        fcntl(cs_fd, F_SETFL, opts);

        switch (handle_connection(server_info, cs_fd)) {
            case FAILURE:
                fprintf(stderr, "handle_connection: failed!\n");
                // publish connection error event
                server_info_connection_error_event(server_info, cs_fd, CONN_ERROR_HANDLER, "handle_connection: failed!");
                break;
            case CLOSED:
                printf("handle_connection: closed!\n");
                // publish client disconnecting event
                server_info_client_disconnecting_event(server_info, cs_fd);
                break;
            default:
                break;
        }

        if(close(cs_fd) < 0){
            fprintf(stderr, "close: %s\n", strerror(errno));
            server_info_connection_error_event(server_info, cs_fd, CONN_ERROR_CLOSE, strerror(errno));
            return FAILURE;
        }
    }
}

/**
 * Function is looping infinitely and waiting
 * for new incoming client connections.
 * It handles each connection on concurrent thread.
 */
result_t concurrent_stream_server_loop(server_info_t *server_info, connection_handler_t conn_handler) {

    sock_fd_t cs_fd, ps_fd;
    pthread_t conn_thread;

    // get passive server socket
    ps_fd = server_info_sock(server_info);

    while(1) {

        if(server_info_should_shut_down(server_info)) {
            return CLOSED;
        }
        if(server_info_force_shut_down(server_info)) {
            return FORCE_CLOSED;
        }

       // check to accept new connection on the main thread...
        cs_fd = accept_new_connection(ps_fd);

        if(cs_fd == FAILURE) {
            fprintf(stderr, "accept_new_connection: failed!\n");
            server_info_connection_error_event(server_info, cs_fd, CONN_ERROR_ACCEPT, "accept_new_connection: failed!");
            return FAILURE;
        } else if(cs_fd == CONTINUE) {
            continue;
        }

        // publish client connected event
        server_info_client_connected_event(server_info, cs_fd);

        // handle new connection on concurrent thread
        if( (conn_thread = connection_thread(conn_handler, server_info, cs_fd)) == NULL ) {
            fprintf(stderr, "connection_thread: failed!\n");
            server_info_connection_error_event(server_info, cs_fd, CONN_ERROR_THREAD, "connection_thread: failed!");
            close(cs_fd);
            continue;
        }

        pthread_detach(conn_thread); // don't care later about joining (releasing) connection thread
    }
}

/**
 * Function is looping infinitely and waiting
 * for new incoming client connections.
 * It handles each connection on managed concurrent thread.
 */
result_t managed_concurrent_stream_server_loop(server_info_t *server_info, connection_handler_t conn_handler) {

    sock_fd_t cs_fd, ps_fd;
    threads_manager_t *threads_manager;

    // get passive server socket
    ps_fd = server_info_sock(server_info);

    // initialize threads manager and set max number of child threads that can be concurrently running
    threads_manager_init(&threads_manager, 10);

    while(1) {

        if(server_info_should_shut_down(server_info)) {
            threads_manager_free(threads_manager);
            return CLOSED;
        }
        if(server_info_force_shut_down(server_info)) {
            threads_manager_free(threads_manager);
            return FORCE_CLOSED;
        }

        // check to accept new connections on the main thread...
        cs_fd = accept_new_connection(ps_fd);

        if(cs_fd == FAILURE) {
            fprintf(stderr, "accept_new_connection: failed!\n");
            server_info_connection_error_event(server_info, cs_fd, CONN_ERROR_ACCEPT, "accept_new_connection: failed!");
            break;
        } else if(cs_fd == CONTINUE) {
            continue;
        }

        // publish client connected event
        server_info_client_connected_event(server_info, cs_fd);

        // handle new connection on concurrent managed thread
        if(timed_wait_for_connection_thread(threads_manager, 3000 /* [ms] */,  conn_handler, server_info, cs_fd) == FAILURE) {
            fprintf(stderr, "timed_wait_for_connection_thread: timed out not obtaining thread for connection handling.\n");
            server_info_connection_error_event(server_info, cs_fd, CONN_ERROR_THREAD, "timed_wait_for_connection_thread: timed out not obtaining thread for connection handling.");
            close(cs_fd);
            continue;
        }
    }

    threads_manager_free(threads_manager);
    return FAILURE;
}

/**
 * Function is looping infinitely and waiting
 * for new incoming connections.
 * It handles each connection on thread pool's worker thread
 */
result_t thread_pool_stream_server_loop(server_info_t *server_info, connection_handler_t conn_handler) {

    sock_fd_t cs_fd, ps_fd;
    thread_pool_t *thread_pool;

    // get passive server socket
    ps_fd = server_info_sock(server_info);

    // initialize thread pool and set its size
    thread_pool_init(&thread_pool, 5, 10, 3000 /* [ms] */);

    while(1) {

        if(server_info_should_shut_down(server_info)) {
            thread_pool_shutdown(thread_pool);
            return CLOSED;
        }
        if(server_info_force_shut_down(server_info)) {
            thread_pool_shutdown(thread_pool);
            return FORCE_CLOSED;
        }

        // check to accept new connections on the main thread...
        cs_fd = accept_new_connection(ps_fd);

        if(cs_fd == FAILURE) {
            fprintf(stderr, "accept_new_connection: failed!\n");
            server_info_connection_error_event(server_info, cs_fd, CONN_ERROR_ACCEPT, "accept_new_connection: failed!");
            break;
        } else if(cs_fd == CONTINUE) {
            continue;
        }

        // publish client connected event
        server_info_client_connected_event(server_info, cs_fd);

        // handle new connection by thread pool's worker thread
        conn_thread_runner_attr_t *connection_thread_runner_attr;
        conn_thread_runner_attr_init(&connection_thread_runner_attr);
        conn_thread_runner_attr_fill(connection_thread_runner_attr, conn_handler, server_info, cs_fd, NULL, NULL);

        thread_pool_run(thread_pool, (runner_t) connection_thread_runner, (runner_attr_t) connection_thread_runner_attr, NULL);

        // adjust thread pool actual size to number of added connection handling tasks
        thread_pool_adjust_size(thread_pool);
    }

    thread_pool_force_free(thread_pool);
    return FAILURE;
}

/**
 * request_handler_t has the same prototype as connection_handler_t,
 * but in contrast to connection_handler_t it is intended for
 * handling just a single request from a client instead of all connection
 * consisting of multiple client requests.
 */
static void close_all_connection_sockets(server_info_t *server_info, fd_set *all_fds, int max_fd, sock_fd_t ps_fd) {

    for(sock_fd_t cs_fd = 0; cs_fd <= max_fd; cs_fd++) {
        if (cs_fd != ps_fd && FD_ISSET(cs_fd, all_fds)) {

            // publish client disconnecting event
            server_info_client_disconnecting_event(server_info, cs_fd);

            if(close(cs_fd) < 0) {
                fprintf(stderr, "close: %s\n", strerror(errno));
                server_info_connection_error_event(server_info, cs_fd, CONN_ERROR_CLOSE, strerror(errno));
            }
            printf("connection socket: %d has been closed.\n", cs_fd);

            FD_CLR(cs_fd, all_fds);
        }
    }
}

result_t pseudo_concurrent_stream_server_loop(server_info_t *server_info, request_handler_t handle_conn_request) {

    int max_fd;
    sock_fd_t ps_fd;            // passive server socket file descriptor
    sock_fd_t cs_fd;            // new connection socket file descriptor
    fd_set all_fds, read_fds;   // all available file descriptors, read file descriptors used to find ready to read fds with select() function
    struct timeval tv;          // time value for select() timeout
    int noOfReadableFDs;        // number of new file descriptors to be read returned by the last select() call

    // 0. get passive server socket
    ps_fd = server_info_sock(server_info);

    // 1. initialize file descriptors set
    max_fd = ps_fd;
    FD_ZERO(&all_fds);
    FD_SET(ps_fd, &all_fds);

    printf("Passive sock: %d, All FDs: %d", ps_fd, all_fds.fds_bits);

    tv.tv_sec = 1; // sleep for one second!
    tv.tv_usec = 0;

    while (1) {

        if(server_info_should_shut_down(server_info)) {
            close_all_connection_sockets(server_info, &all_fds, max_fd, ps_fd);
            return CLOSED;
        }
        if(server_info_force_shut_down(server_info)) {
            close_all_connection_sockets(server_info, &all_fds, max_fd, ps_fd);
            return FORCE_CLOSED;
        }

        printf("Waiting for new readable file descriptors on the main thread...\n");

        // 2. wait for ready to read file descriptors
        memcpy(&read_fds, &all_fds, sizeof(all_fds));
        noOfReadableFDs = select(max_fd+1, &read_fds, NULL, NULL, &tv);
        if( noOfReadableFDs == FAILURE ) {
            fprintf(stderr, "select: failed with %s\n", strerror(errno));
            server_info_server_error_event(server_info, SERVER_ERROR_SELECT,  strerror(errno));
            return FAILURE;
        } else if(noOfReadableFDs > 0) {
            printf("There are new FDs ready to be read.\n");
        } else {
            printf("select() timed out without new FDs to be read.\n");
            continue;
        }

        // 3. is new connection available?
        if(FD_ISSET(ps_fd, &read_fds)) {
            if( (cs_fd = accept_new_connection(ps_fd)) == FAILURE ) {
                fprintf(stderr, "accept_new_connection: failed!\n");
                server_info_connection_error_event(server_info, cs_fd, CONN_ERROR_ACCEPT, "accept_new_connection: failed!");
                return FAILURE;
            }
            FD_SET(cs_fd, &all_fds);
            if(cs_fd > max_fd) max_fd = cs_fd;

            // publish client connected event
            server_info_client_connected_event(server_info, cs_fd);
        }

        // 4. handle requests on pseudo-concurrently available connections
        for(cs_fd = 0; cs_fd <= max_fd; cs_fd++) {
            if(cs_fd != ps_fd && FD_ISSET(cs_fd, &read_fds)) {
                switch ( handle_conn_request(cs_fd) ) {
                    case CLOSED:

                        // publish client disconnecting event
                        server_info_client_disconnecting_event(server_info, cs_fd);

                        if(close(cs_fd) < 0) {
                            fprintf(stderr, "close: %s\n", strerror(errno));
                            server_info_connection_error_event(server_info, cs_fd, CONN_ERROR_CLOSE, strerror(errno));
                            return FAILURE;
                        }
                        printf("connection socket: %d has been closed.\n", cs_fd);
                        FD_CLR(cs_fd, &all_fds);

                        break;

                    case FAILURE:
                        fprintf(stderr, "handle_conn_request: failed for conn socket: %d.\n", cs_fd);
                        server_info_connection_error_event(server_info, cs_fd, CONN_ERROR_HANDLER, "handle_conn_request: failed for conn socket");
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

#define MAX_BUF_SIZE 256

// UDP
result_t iterative_datagram_server_loop(server_info_t *server_info, datagram_handler_t handle_datagram) {

    sock_fd_t ps_fd;
    char buf[MAX_BUF_SIZE];
    int n_recv; // number of bytes received
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);

    // get passive server socket
    ps_fd = server_info_sock(server_info);

    while(1) {

        if(server_info_should_shut_down(server_info) || server_info_force_shut_down(server_info)) {
            return CLOSED;
        }

        // check to receive a new datagram on the main thread...

        if( (n_recv = recvfrom(ps_fd, buf, sizeof(buf)-1, 0, (struct sockaddr *) &peer_addr, &peer_addr_len)) < 0 ) {
            if( (errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                continue;
            }
            fprintf(stderr, "recvfrom: %s\n", strerror(errno));
            server_info_datagram_error_event(server_info, DATAGRAM_ERROR_RECVFROM, strerror(errno));
            return FAILURE;
        }
        buf[n_recv] = '\0';

        printf("Handle datagram on the main thread...\n");

        if(handle_datagram(ps_fd, (struct sockaddr *) &peer_addr, buf) == FAILURE) {
            fprintf(stderr, "handle_datagram: %s\n", strerror(errno));
            server_info_datagram_error_event(server_info, DATAGRAM_ERROR_HANDLER, strerror(errno));
            continue;
        }
    }
}

result_t concurrent_datagram_server_loop(server_info_t *server_info, datagram_handler_t datagram_handler) {

    sock_fd_t ps_fd;
    char buf[MAX_BUF_SIZE];
    int n_recv; // number of bytes received
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);

    pthread_t thread;

    // get passive server socket
    ps_fd = server_info_sock(server_info);

    while(1) {

        if(server_info_should_shut_down(server_info) || server_info_force_shut_down(server_info)) {
            return CLOSED;
        }

        // check to receive a new datagram on the main thread...

        if( (n_recv = recvfrom(ps_fd, buf, sizeof(buf)-1, 0, (struct sockaddr *) &peer_addr, &peer_addr_len)) < 0 ) {
            if( (errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                continue;
            }
            fprintf(stderr, "recvfrom: %s\n", strerror(errno));
            server_info_datagram_error_event(server_info, DATAGRAM_ERROR_RECVFROM, strerror(errno));
            return FAILURE;
        }
        buf[n_recv] = '\0';

        // handle datagram on concurrent thread
        if( (thread = datagram_thread(datagram_handler, ps_fd, (struct sockaddr *) &peer_addr, buf)) == NULL ) {
            fprintf(stderr, "datagram_thread: failed!\n");
            server_info_datagram_error_event(server_info, DATAGRAM_ERROR_THREAD, "datagram_thread: failed!");
            continue;
        }
        pthread_detach(thread); // don't care later about joining (releasing) datagram thread
    }
}

result_t managed_concurrent_datagram_server_loop(server_info_t *server_info, datagram_handler_t datagram_handler) {

    sock_fd_t ps_fd;
    char buf[MAX_BUF_SIZE];
    int n_recv; // number of bytes received
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);

    threads_manager_t *threads_manager;

    // get passive server socket
    ps_fd = server_info_sock(server_info);

    // initialize threads manager and set max number of child threads that can be concurrently running
    threads_manager_init(&threads_manager, 10);

    while(1) {

        if(server_info_should_shut_down(server_info) || server_info_force_shut_down(server_info)) {
            threads_manager_free(threads_manager);
            return CLOSED;
        }

        // check to receive a new datagram on the main thread...

        if( (n_recv = recvfrom(ps_fd, buf, sizeof(buf)-1, 0, (struct sockaddr *) &peer_addr, &peer_addr_len)) < 0 ) {
            if( (errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                continue;
            }
            fprintf(stderr, "recvfrom: %s\n", strerror(errno));
            server_info_datagram_error_event(server_info, DATAGRAM_ERROR_RECVFROM, strerror(errno));
            break;
        }
        buf[n_recv] = '\0';

        // handle datagram on concurrent managed thread
        if(timed_wait_for_datagram_thread(threads_manager, 3000 /* [ms] */,  datagram_handler, ps_fd, (struct sockaddr *) &peer_addr, buf) == FAILURE) {
            fprintf(stderr, "timed_wait_for_datagram_thread: timed out not obtaining thread for datagram handling.\n");
            server_info_datagram_error_event(server_info, DATAGRAM_ERROR_THREAD, "timed_wait_for_datagram_thread: timed out not obtaining thread for datagram handling.");
            continue;
        }
    }

    threads_manager_free(threads_manager);
    return FAILURE;
}

result_t thread_pool_datagram_server_loop(server_info_t *server_info, datagram_handler_t datagram_handler) {

    sock_fd_t ps_fd;
    char buf[MAX_BUF_SIZE];
    int n_recv; // number of bytes received
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);

    thread_pool_t *thread_pool;

    // get passive server socket
    ps_fd = server_info_sock(server_info);

    // initialize thread pool and set its size
    thread_pool_init(&thread_pool, 5, 10, 3000 /* [ms] */);

    while(1) {

        if(server_info_should_shut_down(server_info)) {
            thread_pool_shutdown(thread_pool);
            return CLOSED;
        }
        if(server_info_force_shut_down(server_info)) {
            thread_pool_force_free(thread_pool);
            return FORCE_CLOSED;
        }

        // check to receive a new datagram on the main thread...

        if( (n_recv = recvfrom(ps_fd, buf, sizeof(buf)-1, 0, (struct sockaddr *) &peer_addr, &peer_addr_len)) < 0 ) {
            if( (errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                continue;
            }
            fprintf(stderr, "recvfrom: %s\n", strerror(errno));
            server_info_datagram_error_event(server_info, DATAGRAM_ERROR_RECVFROM, strerror(errno));
            break;
        }
        buf[n_recv] = '\0';

        // handle datagram by thread pool's worker thread
        datagram_thread_runner_attr_t *datagram_thread_runner_attr;
        datagram_thread_runner_attr_init(&datagram_thread_runner_attr);
        datagram_thread_runner_attr_fill(datagram_thread_runner_attr, datagram_handler, ps_fd, (struct sockaddr *) &peer_addr, buf, NULL, NULL);

        thread_pool_run(thread_pool, (runner_t) datagram_thread_runner, (runner_attr_t) datagram_thread_runner_attr, NULL);

        // adjust thread pool actual size to number of added connection handling tasks
        thread_pool_adjust_size(thread_pool);
    }

    thread_pool_force_free(thread_pool);
    return FAILURE;
}