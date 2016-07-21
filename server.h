/*
 * Created by Michal Ziobro on 21/07/2016.
 */

#ifndef REMOTECONTROLLERAPI_SERVER_SERVER_H
#define REMOTECONTROLLERAPI_SERVER_SERVER_H

#define PORT "2016" // the port remote controller server will be listening for connections
#define BACKLOG 10  // number of pending connections that can be waiting on the listen queue

enum result {
    SUCCESS = 0,
    FAILURE = -1,
};

int start_server(void);
int create_passive_socket(int *res_fd);
int listen_connections(void);
int accept_new_connection(void);
int end_server(void);


#endif /* REMOTECONTROLLERAPI_SERVER_SERVER_H */
