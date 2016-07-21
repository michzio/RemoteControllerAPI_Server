#include <stdio.h>
#include "server.h"

int main(int argc, char **argv) {

    printf("Remote Controller Server - desktop side, remotely controlled.\n");

    start_server();

    return 0;
}