#ifndef RING_BUFFER_ARENA_H
#define RING_BUFFER_ARENA_H

#include <stddef.h>
#include "arena.h"

typedef struct ring_buffer_arena ring_buffer_arena;

ring_buffer_arena *ring_buffer_arena_create(
    arena *a,
    size_t capacity,
    size_t datasize
);

void ring_buffer_arena_put(
    ring_buffer_arena *buf,
    void *data,
    size_t data_size
);

void ring_buffer_arena_peek(
    ring_buffer_arena *buf,
    void *data,
    size_t data_size
);

void ring_buffer_arena_pop(ring_buffer_arena *buf);

void ring_buffer_arena_get(
    ring_buffer_arena *buf,
    size_t index,
    void *data,
    size_t data_size
);

void ring_buffer_arena_set(
    ring_buffer_arena *buf,
    size_t index,
    void *data,
    size_t data_size
);

size_t ring_buffer_arena_size(ring_buffer_arena *buf);
size_t ring_buffer_arena_cap(ring_buffer_arena *buf);

#endif