#include <stdio.h>
#include "server.h"
#include "test/test_doubly_linked_list.h"
#include "test/test_fifo_queue.h"
#include "test/test_task_queue.h"
#include "test/test_array_helper.h"
#include "test/test_thread_pool.h"
#include "test/test_threads_manager.h"

int main(int argc, char **argv) {

    printf("Remote Controller Server - desktop side, remotely controlled.\n");

    //test_doubly_linked_list.run_tests();
    //test_fifo_queue.run_tests();
    //test_array_helper.run_tests();
    //test_task_queue.run_tests();
    //test_thread_pool.run_tests();
    test_threads_manager.run_tests();
    //start_server();

    return 0;
}