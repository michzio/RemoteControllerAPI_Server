//
// Created by Michal Ziobro on 08/08/2016.
//

#ifndef REMOTECONTROLLERAPI_SERVER_TIME_H
#define REMOTECONTROLLERAPI_SERVER_TIME_H

#define MSEC_IN_SEC 1000
#define USEC_IN_SEC 1000*1000
#define USEC_IN_MSEC 1000
#define NSEC_IN_SEC 1000*1000*1000
#define NSEC_IN_MSEC 1000*1000
#define NSEC_IN_USEC 1000

void set_timespec_from_timeout(struct timespec *timespec, int ms_timeout);
long timeval_difference(const struct timeval *timeval_1, const struct timeval *timeval_2);

#endif //REMOTECONTROLLERAPI_SERVER_TIME_H
