#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

typedef struct arena arena;

arena *arena_create(size_t capacity);
void arena_destroy(arena **parena);

void *arena_alloc(arena *a, size_t size);

void arena_reset(arena *a);

size_t arena_used(arena *a);
size_t arena_cap(arena *a);

#endif