#include "common.h"
#include "plant.h"
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void usage(const char *prog) {
    fprintf(stderr, "Uso: %s [--duration S] [--Ts S] [--out DIR] [--load-ms MS]\n", prog);
}

int main(int argc, char **argv) {
    rt_params_t cfg = { .Ts = 0.05, .duration = 20.0, .D = 0.30, .load_ms = 0 };
    snprintf(cfg.outdir, sizeof(cfg.outdir), "./out");

    for (int i=1; i<argc; ++i) {
        if (strcmp(argv[i],"--duration")==0 && i+1<argc) { cfg.duration = atof(argv[++i]); }
        else if (strcmp(argv[i],"--Ts")==0 && i+1<argc)   { cfg.Ts = atof(argv[++i]); }
        else if (strcmp(argv[i],"--out")==0 && i+1<argc)  { snprintf(cfg.outdir, sizeof(cfg.outdir), "%s", argv[++i]); }
        else if (strcmp(argv[i],"--load-ms")==0 && i+1<argc) { cfg.load_ms = atoi(argv[++i]); }
        else { usage(argv[0]); return 1; }
    }

    // prepara barramento compartilhado
    shared_bus_t bus;
    pthread_mutex_init(&bus.mtx, NULL);
    bus.u.v = 0.0; bus.u.w = 0.0;
    bus.y.x = 0.0; bus.y.y = 0.0; bus.y.th = 0.0;

    thread_ctx_t ctx = { .bus = &bus, .cfg = &cfg };

    pthread_t th_plant, th_io;
    if (pthread_create(&th_plant, NULL, plant_thread, &ctx) != 0) { perror("pthread_create plant"); return 1; }
    if (pthread_create(&th_io,    NULL, io_thread,    &ctx) != 0) { perror("pthread_create io"); return 1; }

    pthread_join(th_io, NULL);
    pthread_join(th_plant, NULL);

    pthread_mutex_destroy(&bus.mtx);
    return 0;
}
