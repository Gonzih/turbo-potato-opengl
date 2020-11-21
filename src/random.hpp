#pragma once

#include <time.h>
#include <stdlib.h>

namespace rng
{
    inline void init() {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);

        srand((time_t)ts.tv_nsec);
    }

    inline int gen_int(int lower, int upper) {
        return rand() % (upper - lower) + lower;
    }
};
