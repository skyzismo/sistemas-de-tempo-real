#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>
#include <stdbool.h>

typedef struct {
    double v;     // velocidade linear
    double w;     // velocidade angular
} input_u_t;

typedef struct {
    double x;     // x frontal
    double y;     // y frontal
    double th;    // orientação
} output_y_t;

typedef struct {
    pthread_mutex_t mtx;
    input_u_t  u;     // troca (io -> plant)
    output_y_t y;     // troca (plant -> io)
} shared_bus_t;

typedef struct {
    double Ts;         // período nominal [s]
    double duration;   // horizonte [s]
    double D;          // diâmetro [m]
    int    load_ms;    // carga artificial por ciclo [ms]
    char   outdir[256];
} rt_params_t;

typedef struct {
    shared_bus_t *bus;
    rt_params_t  *cfg;
} thread_ctx_t;

void busy_work_ms(int ms);

#endif // COMMON_H
