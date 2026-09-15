#include "buffer_arena.h"
#include <string.h>

struct buffer_arena
{
    unsigned char *data;

    size_t size;
    size_t cap;
    size_t datasize;
};

buffer_arena *buffer_arena_create(
    arena *a,
    size_t capacity,
    size_t datasize
)
{
    if (a == NULL || capacity == 0 || datasize == 0)
    {
        return NULL;
    }

    buffer_arena *buf =
        arena_alloc(a, sizeof(buffer_arena));

    if (buf == NULL)
    {
        return NULL;
    }

    buf->data =
        arena_alloc(a, capacity * datasize);

    if (buf->data == NULL)
    {
        return NULL;
    }

    buf->size = 0;
    buf->cap = capacity;
    buf->datasize = datasize;

    return buf;
}

void buffer_arena_put(
    buffer_arena *buf,
    void *data,
    size_t data_size
)
{
    if (buf == NULL || data == NULL)
    {
        return;
    }

    if (buf->size >= buf->cap)
    {
        return;
    }

    if (data_size != buf->datasize)
    {
        return;
    }

    memcpy(
        buf->data + (buf->size * buf->datasize),
        data,
        data_size
    );

    buf->size++;
}

void buffer_arena_peek(
    buffer_arena *buf,
    void *data,
    size_t data_size
)
{
    if (buf == NULL || data == NULL)
    {
        return;
    }

    if (buf->size == 0)
    {
        return;
    }

    if (data_size != buf->datasize)
    {
        return;
    }

    memcpy(
        data,
        buf->data,
        data_size
    );
}

void buffer_arena_get(
    buffer_arena *buf,
    size_t index,
    void *data,
    size_t data_size
)
{
    if (buf == NULL || data == NULL)
    {
        return;
    }

    if (index >= buf->size)
    {
        return;
    }

    if (data_size != buf->datasize)
    {
        return;
    }

    memcpy(
        data,
        buf->data + (index * buf->datasize),
        data_size
    );
}

void buffer_arena_set(
    buffer_arena *buf,
    size_t index,
    void *data,
    size_t data_size
)
{
    if (buf == NULL || data == NULL)
    {
        return;
    }

    if (index >= buf->size)
    {
        return;
    }

    if (data_size != buf->datasize)
    {
        return;
    }

    memcpy(
        buf->data + (index * buf->datasize),
        data,
        data_size
    );
}

size_t buffer_arena_size(buffer_arena *buf)
{
    if (buf == NULL)
    {
        return 0;
    }

    return buf->size;
}

size_t buffer_arena_cap(buffer_arena *buf)
{
    if (buf == NULL)
    {
        return 0;
    }

    return buf->cap;
}