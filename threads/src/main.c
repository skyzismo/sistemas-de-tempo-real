#include<assert.h>
#include<stdalign.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<threads.h>
#include<time.h>

#define MS 1000000

typedef struct{
    size_t len, cap;
    unsigned char *memo;
}arena;


alignas(max_align_t) unsigned char memory[1024] = {0};

arena a = {.len = 0, .cap = 1024, .memo = memory};

mtx_t mtx = {0};
cnd_t cnd_full = {0};
cnd_t cnd_empty = {0};

typedef struct{
    size_t size, cap;
    size_t head, tail;
    int *data;
}ring;

#define MAX_RING_SIZE 5

void ring_add(ring *r, int *value){
    assert(r); //pre-condicao caso o r seja nulo
    mtx_lock(&mtx); //semaforo binario

    while(r->size >= r->cap){
        cnd_wait(&cnd_full, &mtx); //operacoes atomicas, transformadas em linha de assembly
    }

    r->data[r->tail] = *value;
    r->tail = (r->tail + 1) % r->cap;
    r->size++;

    cnd_signal(&cnd_empty);
    mtx_unlock(&mtx);
}

void ring_remove(ring *r, int *value){
    assert(r); //pre-condicao
    mtx_lock(&mtx);

    while(r->size == 0){
        cnd_wait(&cnd_empty, &mtx);
    }

    *value = r->data[r->head];
    r->head = (r->head + 1) % r->cap;
    r->size--;

    cnd_signal(&cnd_full);

    mtx_unlock(&mtx);
}

void ring_display(ring *r){
    //foo
}

int task_prod(void *arg){
    printf("Produtor\n");

    ring *r = (ring*)arg; // ???

    while(1) {
        int num = rand() % 100;

        ring_add(r, &num);

        printf("Produzido\n");

        struct timespec ts = {.tv_sec = 0, .tv_nsec =500*MS}; //??

        thrd_sleep(&ts, NULL);
    }

    return 0;
}

int task_cons(void *arg){
    printf("Consumidor\n");

    ring *r = (ring*)arg; // ???

    while(1) {
        int num = rand() % 100;

        ring_remove(r, &num);

        printf("Consumido\n");

        struct timespec ts = {.tv_sec = 0, .tv_nsec =550*MS}; //??

        thrd_sleep(&ts, NULL);
    }

    return 0;
}

int task_display(void *arg){
    printf("Exibidor\n");

    ring *r = (ring*)arg; // ???

    while(1) {

        ring_print(r);

        struct timespec ts = {.tv_sec = 0, .tv_nsec = 500*MS}; //??

        thrd_sleep(&ts, NULL);
    }

    return 0;
}



//TODO: implementar a main para realizar o test do ring com multi-threads

int main()
{
    /* code */

    mtx_init(&mtx, mtx_plain);
    cnd_init(&cnd_full);
    cnd_init(&cnd_empty);

    ring r = {.size = 0, .cap = MAX_RING_SIZE, .head = 0, .tail = 0, .data = (int *)(a.memo + a.len)};

    

    cnd_destroy(&cnd_empty);
    cnd_destroy(&cnd_full);
    mtx_destroy(&mtx);
    return 0;
}
