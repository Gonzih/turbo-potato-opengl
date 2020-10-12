#ifndef TP_RANDOM_H
#define TP_RANDOM_H

#include <time.h>
#include <stdlib.h>

void rand_init() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    srand((time_t)ts.tv_nsec);
}

int rand_int(int lower, int upper) {
    return rand() % (upper - lower) + lower;
}

#endif
