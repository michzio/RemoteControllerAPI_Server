//
// Created by Michal Ziobro on 26/07/2016.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "server_loop.h"
#include "../concurrency/thread.h"
#include "../concurrency/threads_manager.h"
#include "../concurrency/thread_pool/thread_pool.h"

// TCP

/**
 * Function is looping infinitely and waiting
 * for new incoming client connections.
 * It handles connections one by one.
 */
result_t iterative_stream_server_loop(sock_fd_t ps_fd, connection_handler_t handle_connection) {

    int cs_fd;

    printf("Waiting for new connections on the main thread...\n");

    while(1) {
        cs_fd = accept_new_connection(ps_fd);

        if(cs_fd == FAILURE) {
            fprintf(stderr, "accept_new_connection: failed!\n");
            return FAILURE;
        } else if(cs_fd == CONTINUE) {
            continue;
        }

        printf("Handle connection on the main thread...\n");

        if(handle_connection(cs_fd) < 0) {
            fprintf(stderr, "handle_connection: failed!\n");
        }

        if(close(cs_fd) < 0){
            fprintf(stderr, "close: %s\n", strerror(errno));
            return FAILURE;
        }
    }
}

result_t concurrent_stream_server_loop(sock_fd_t ps_fd, connection_handler_t conn_handler) {

    int cs_fd;
    pthread_t conn_thread;

    printf("Waiting for new connections on the main thread...\n");

    while(1) {

        cs_fd = accept_new_connection(ps_fd);

        if(cs_fd == FAILURE) {
            fprintf(stderr, "accept_new_connection: failed!\n");
            return FAILURE;
        } else if(cs_fd == CONTINUE) {
            continue;
        }

        // handle new connection on concurrent thread
        if( (conn_thread = connection_thread(conn_handler, cs_fd)) == NULL ) {
            fprintf(stderr, "connection_thread: failed!\n");
            close(cs_fd);
            continue;
        }
        pthread_detach(conn_thread); // don't care later about joining (releasing) connection thread
    }
}

result_t managed_concurrent_stream_server_loop(sock_fd_t ps_fd, connection_handler_t conn_handler) {

    int cs_fd;
    threads_manager_t *threads_manager;

    printf("Waiting for new connections on the main thread...\n");

    // initialize threads manager and set max number of child threads that can be concurrently running
    threads_manager_init(&threads_manager, 10);

    while(1) {

        cs_fd = accept_new_connection(ps_fd);

        if(cs_fd == FAILURE) {
            fprintf(stderr, "accept_new_connection: failed!\n");
            break;
        } else if(cs_fd == CONTINUE) {
            continue;
        }

        // handle new connection on concurrent managed thread
        if(timed_wait_for_connection_thread(threads_manager, 3000 /* [ms] */,  conn_handler, cs_fd) == FAILURE) {
            fprintf(stderr, "timed_wait_for_connection_thread: timed out not obtaining thread for connection handling.\n");
            close(cs_fd);
            continue;
        }
    }

    threads_manager_free(threads_manager);
    return FAILURE;
}

result_t thread_pool_stream_server_loop(sock_fd_t ps_fd, connection_handler_t conn_handler) {

    int cs_fd;
    thread_pool_t *thread_pool;

    printf("Waiting for new connections on the main thread...\n");

    // initialize thread pool and set its size
    thread_pool_init(&thread_pool, 5, 10, 3000 /* [ms] */);

    while(1) {

        cs_fd = accept_new_connection(ps_fd);

        if(cs_fd == FAILURE) {
            fprintf(stderr, "accept_new_connection: failed!\n");
            break;
        } else if(cs_fd == CONTINUE) {
            continue;
        }

        // handle new connection by thread pool's worker thread
        conn_thread_runner_attr_t *connection_thread_runner_attr;
        conn_thread_runner_attr_init(&connection_thread_runner_attr);
        conn_thread_runner_attr_fill(connection_thread_runner_attr, conn_handler, cs_fd, NULL, NULL);

        thread_pool_run(thread_pool, (runner_t) connection_thread_runner, (runner_attr_t) connection_thread_runner_attr, NULL);

        // adjust thread pool actual size to number of added connection handling tasks
        thread_pool_adjust_size(thread_pool);
    }

    thread_pool_force_free(thread_pool);
    return FAILURE;
}

result_t pseudo_concurrent_stream_server_loop(sock_fd_t ps_fd, connection_handler_t conn_handler) {

    // TODO

    return SUCCESS;
}

#define MAX_BUF_SIZE 256

// UDP
result_t iterative_datagram_server_loop(sock_fd_t ps_fd, datagram_handler_t handle_datagram) {

    char buf[MAX_BUF_SIZE];
    int n_recv; // number of bytes received
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);

    printf("Waiting for new datagrams on the main thread...\n");

    while(1) {
        if( (n_recv = recvfrom(ps_fd, buf, sizeof(buf)-1, 0, (struct sockaddr *) &peer_addr, &peer_addr_len)) < 0 ) {
            fprintf(stderr, "recvfrom: %s\n", strerror(errno));
            return FAILURE;
        }
        buf[n_recv] = '\0';

        printf("Handle datagram on the main thread...\n");

        if(handle_datagram(ps_fd, (struct sockaddr *) &peer_addr, buf) == FAILURE) {
            fprintf(stderr, "handle_datagram: %s\n", strerror(errno));
            continue;
        }
    }
}

result_t concurrent_datagram_server_loop(sock_fd_t ps_fd, datagram_handler_t datagram_handler) {

    // TODO

    return SUCCESS;
}

result_t managed_concurrent_datagram_server_loop(sock_fd_t ps_fd, datagram_handler_t datagram_handler) {

    return SUCCESS;
}

result_t thread_pool_datagram_server_loop(sock_fd_t ps_fd, datagram_handler_t datagram_handler) {

    // TODO

    return SUCCESS;
}