#define _GNU_SOURCE 1
#include "plant.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

static inline double timespec_to_s(const struct timespec *t) {
    return (double)t->tv_sec + (double)t->tv_nsec * 1e-9;
}

void *plant_thread(void *arg) {
    thread_ctx_t *ctx = (thread_ctx_t*)arg;
    shared_bus_t *bus = ctx->bus;
    rt_params_t  *cfg = ctx->cfg;

    // estados do robô no centro de massa
    double xc = 0.0, yc = 0.0, th = 0.0;

    char path_timing[512];
    snprintf(path_timing, sizeof(path_timing), "%s/timing_plant_%s.csv",
             cfg->outdir, (cfg->load_ms>0) ? "load_%dms" : "no_load");
    if (cfg->load_ms>0) {
        // completa o nome com o valor
        snprintf(path_timing, sizeof(path_timing), "%s/timing_plant_load_%dms.csv",
                 cfg->outdir, cfg->load_ms);
    }

    FILE *ft = fopen(path_timing, "w");
    if (!ft) { perror("fopen timing plant"); return NULL; }
    fprintf(ft, "t_release\n");

    struct timespec t0;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    // alinha para próximo tick
    long ns_period = (long)llround(cfg->Ts * 1e9);
    struct timespec next = t0;
    next.tv_nsec = ((next.tv_nsec / ns_period) + 1) * ns_period;
    if (next.tv_nsec >= 1000000000L) { next.tv_sec += 1; next.tv_nsec -= 1000000000L; }

    double t_sim = 0.0;
    size_t steps = (size_t)ceil(cfg->duration / cfg->Ts) + 1;

    for (size_t k = 0; k < steps; ++k) {
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
        struct timespec trel;
        clock_gettime(CLOCK_MONOTONIC, &trel);
        fprintf(ft, "%.9f\n", timespec_to_s(&trel));
        fflush(ft);

        // lê u(t) (apenas os 2 valores)
        double v, w;
        pthread_mutex_lock(&bus->mtx);
        v = bus->u.v;
        w = bus->u.w;
        pthread_mutex_unlock(&bus->mtx);

        // integração de Euler do modelo:
        // xdot = [ sin(th)*v, cos(th)*v, w ]
        xc += cfg->Ts * (sin(th) * v);
        yc += cfg->Ts * (cos(th) * v);
        th += cfg->Ts * w;

        // saída frontal y_f
        double yx = xc + 0.5 * cfg->D * cos(th);
        double yy = yc + 0.5 * cfg->D * sin(th);

        // publica y_f
        pthread_mutex_lock(&bus->mtx);
        bus->y.x = yx;
        bus->y.y = yy;
        bus->y.th = th;
        pthread_mutex_unlock(&bus->mtx);

        busy_work_ms(cfg->load_ms);

        // próximo tick
        next.tv_nsec += ns_period;
        while (next.tv_nsec >= 1000000000L) { next.tv_sec += 1; next.tv_nsec -= 1000000000L; }
        t_sim += cfg->Ts;
        if (t_sim >= cfg->duration) break;
    }

    fclose(ft);
    return NULL;
}
