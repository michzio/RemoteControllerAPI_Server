//
// Created by Michal Ziobro on 28/07/2016.
//

#include "task_queue.h"
#include "../../common/fifo_queue.h"

// TASK QUEUE
struct task_queue {
    fifo_queue_t *fifo;
    int task_count;
};

// TASK
struct task {
    runner_t runner;
    runner_attr_t runner_attr;
    runner_res_t runner_res;
};

// task queue operations
