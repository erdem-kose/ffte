#include <stdio.h>
#include <time.h>
#include "exectime.h"

struct timespec begin, end;

void tic()
{
    clock_gettime(CLOCK_MONOTONIC, &begin);
}

double toc()
{
    clock_gettime(CLOCK_MONOTONIC, &end);
    return ((end.tv_nsec - begin.tv_nsec) / (double)1e9) + (end.tv_sec  - begin.tv_sec);
}