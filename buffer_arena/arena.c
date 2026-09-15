#include "arena.h"
#include <stdlib.h>

struct arena
{
    unsigned char *data;
    size_t capacity;
    size_t offset;
};

arena *arena_create(size_t capacity)
{
    if (capacity == 0)
    {
        return NULL;
    }

    arena *a = malloc(sizeof(arena));

    if (a == NULL)
    {
        return NULL;
    }

    a->data = malloc(capacity);

    if (a->data == NULL)
    {
        free(a);
        return NULL;
    }

    a->capacity = capacity;
    a->offset = 0;

    return a;
}

void arena_destroy(arena **parena)
{
    if (parena == NULL || *parena == NULL)
    {
        return;
    }

    arena *a = *parena;

    free(a->data);
    free(a);

    *parena = NULL;
}

void *arena_alloc(arena *a, size_t size)
{
    if (a == NULL || size == 0)
    {
        return NULL;
    }

    if (a->offset + size > a->capacity)
    {
        return NULL;
    }

    void *ptr = a->data + a->offset;

    a->offset += size;

    return ptr;
}

void arena_reset(arena *a)
{
    if (a == NULL)
    {
        return;
    }

    a->offset = 0;
}

size_t arena_used(arena *a)
{
    if (a == NULL)
    {
        return 0;
    }

    return a->offset;
}

size_t arena_cap(arena *a)
{
    if (a == NULL)
    {
        return 0;
    }

    return a->capacity;
}