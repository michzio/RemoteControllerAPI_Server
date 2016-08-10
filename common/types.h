//
// Created by Michal Ziobro on 31/07/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_TYPES_H
#define REMOTECONTROLLERAPI_SERVER_TYPES_H

typedef enum {
    SUCCESS = 0,
    FAILURE = -1,
    CONTINUE = -2,
} result_t;

typedef void *callback_attr_t;
typedef void (*callback_t)(callback_attr_t callback_attr, void *arg);

#endif //REMOTECONTROLLERAPI_SERVER_TYPES_H
