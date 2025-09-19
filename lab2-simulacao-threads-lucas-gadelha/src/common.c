#include "common.h"
#include <time.h>

void busy_work_ms(int ms) {
    if (ms <= 0) return;
    struct timespec start, now;
    clock_gettime(CLOCK_MONOTONIC, &start);
    long target_ns = (long)ms * 1000000L;
    for (;;) {
        clock_gettime(CLOCK_MONOTONIC, &now);
        long dt = (now.tv_sec - start.tv_sec) * 1000000000L + (now.tv_nsec - start.tv_nsec);
        if (dt >= target_ns) break;
        // ocupação de CPU ingênua
        for (volatile int i = 0; i < 1000; ++i) {}
    }
}
