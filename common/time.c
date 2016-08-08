//
// Created by Michal Ziobro on 08/08/2016.
//

#include <sys/time.h>
#include "time.h"

void set_timespec_from_timeout(struct timespec *timespec, int ms_timeout) {

    struct timeval timeval;
    gettimeofday(&timeval, NULL);

    timespec->tv_sec = time(NULL) + ms_timeout/1000;
    timespec->tv_nsec = timeval.tv_usec * 1000 + 1000 * 1000 * (ms_timeout % 1000);
    timespec->tv_sec += timespec->tv_nsec / (1000 * 1000 * 1000);
    timespec->tv_nsec %= (1000 * 1000 * 1000);
}