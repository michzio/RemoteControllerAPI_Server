#include <stdio.h>
#include "test/test_server_loop.h"
#include "server.h"
#include "test/test_transfer.h"

int main(int argc, char **argv) {

    printf("Remote Controller Server - desktop side, remotely controlled.\n");

    test_server_transfer.run_tests();
    //test_server_loop.run_tests();
    //start_server();

    return 0;
}