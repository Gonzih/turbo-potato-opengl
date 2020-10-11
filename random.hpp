#ifndef TP_RANDOM_H
#define TP_RANDOM_H

#include <time.h>
#include <stdlib.h>

void rand_init() {
    srand (time(NULL));
}

int rand_int(int lower, int upper) {
    return rand() % (upper - lower) + lower;
}

#endif
