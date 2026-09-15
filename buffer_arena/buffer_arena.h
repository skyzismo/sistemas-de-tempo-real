#ifndef BUFFER_ARENA_H
#define BUFFER_ARENA_H

#include <stddef.h>
#include "arena.h"

typedef struct buffer_arena buffer_arena;

buffer_arena *buffer_arena_create(
    arena *a,
    size_t capacity,
    size_t datasize
);

void buffer_arena_put(
    buffer_arena *buf,
    void *data,
    size_t data_size
);

void buffer_arena_peek(
    buffer_arena *buf,
    void *data,
    size_t data_size
);

void buffer_arena_get(
    buffer_arena *buf,
    size_t index,
    void *data,
    size_t data_size
);

void buffer_arena_set(
    buffer_arena *buf,
    size_t index,
    void *data,
    size_t data_size
);

size_t buffer_arena_size(buffer_arena *buf);
size_t buffer_arena_cap(buffer_arena *buf);

#endif