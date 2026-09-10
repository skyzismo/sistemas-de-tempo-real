#include<assert.h>
#include<stdalign.h>
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<threads.h>
#include<time.h>

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
    assert(r); //pre-condicao
    mtx_lock(&mtx);

    while(r->size == r->cap){
        cnd_wait(&cnd_full, &mtx);
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

int task1(void *arg){
    printf("Task 1\n");

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
