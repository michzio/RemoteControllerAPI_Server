#include <stdio.h>
#include "server.h"
#include "test/test_doubly_linked_list.h"
#include "test/test_fifo_queue.h"
#include "test/test_task_queue.h"
#include "test/test_array_helper.h"

int main(int argc, char **argv) {

    printf("Remote Controller Server - desktop side, remotely controlled.\n");

    test_doubly_linked_list.run_tests();
    test_fifo_queue.run_tests();
    test_array_helper.run_tests();
    test_task_queue.run_tests();
    //start_server();

    return 0;
}