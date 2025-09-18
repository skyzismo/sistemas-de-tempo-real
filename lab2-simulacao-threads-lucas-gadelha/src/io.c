#include "io.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

static inline double timespec_to_s(const struct timespec *t) {
    return (double)t->tv_sec + (double)t->tv_nsec * 1e-9;
}

static void u_profile(double t, double *v, double *w) {
    if (t < 0.0) { *v = 0.0; *w = 0.0; return; }
    if (t < 10.0) { *v = 1.0; *w = 0.2 * M_PI; return; }
    *v = 1.0; *w = -0.2 * M_PI;
}

void *io_thread(void *arg) {
    thread_ctx_t *ctx = (thread_ctx_t*)arg;
    shared_bus_t *bus = ctx->bus;
    rt_params_t  *cfg = ctx->cfg;

    // arquivos de saída
    char path_samples[512], path_timing[512];
    if (cfg->load_ms>0) {
        snprintf(path_samples, sizeof(path_samples), "%s/samples_load_%dms.csv", cfg->outdir, cfg->load_ms);
        snprintf(path_timing,  sizeof(path_timing),  "%s/timing_io_load_%dms.csv", cfg->outdir, cfg->load_ms);
    } else {
        snprintf(path_samples, sizeof(path_samples), "%s/samples_no_load.csv", cfg->outdir);
        snprintf(path_timing,  sizeof(path_timing),  "%s/timing_io_no_load.csv", cfg->outdir);
    }
    FILE *fs = fopen(path_samples, "w");
    FILE *ft = fopen(path_timing, "w");
    if (!fs || !ft) { perror("fopen io files"); if (fs) fclose(fs); if (ft) fclose(ft); return NULL; }
    fprintf(fs, "t,v,w,yx,yy,theta\n");
    fprintf(ft, "t_release\n");

    struct timespec t0;
    clock_gettime(CLOCK_MONOTONIC, &t0);
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

        // gera u(t) determinístico pelo tempo teórico k*Ts
        double v, w;
        u_profile(t_sim, &v, &w);

        // publica u
        pthread_mutex_lock(&bus->mtx);
        bus->u.v = v;
        bus->u.w = w;
        // lê y_f
        double yx = bus->y.x;
        double yy = bus->y.y;
        double th = bus->y.th;
        pthread_mutex_unlock(&bus->mtx);

        // armazena linha
        fprintf(fs, "%.6f,%.9f,%.9f,%.9f,%.9f,%.9f\n", t_sim, v, w, yx, yy, th);

        busy_work_ms(cfg->load_ms);

        // próximo tick
        next.tv_nsec += ns_period;
        while (next.tv_nsec >= 1000000000L) { next.tv_sec += 1; next.tv_nsec -= 1000000000L; }
        t_sim += cfg->Ts;
        if (t_sim >= cfg->duration) break;
    }

    fclose(fs);
    fclose(ft);
    return NULL;
}
