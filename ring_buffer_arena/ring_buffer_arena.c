#include "ring_buffer_arena.h"
#include <string.h>

struct ring_buffer_arena
{
    unsigned char *data;

    size_t size;
    size_t cap;
    size_t datasize;

    size_t head;
    size_t tail;
};

ring_buffer_arena *ring_buffer_arena_create(
    arena *a,
    size_t capacity,
    size_t datasize
)
{
    if (a == NULL || capacity == 0 || datasize == 0)
    {
        return NULL;
    }

    ring_buffer_arena *buf =
        arena_alloc(a, sizeof(ring_buffer_arena));

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

    buf->head = 0;
    buf->tail = 0;

    return buf;
}

void ring_buffer_arena_put(
    ring_buffer_arena *buf,
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
        buf->data + (buf->tail * buf->datasize),
        data,
        data_size
    );

    buf->tail =
        (buf->tail + 1) % buf->cap;

    buf->size++;
}

void ring_buffer_arena_peek(
    ring_buffer_arena *buf,
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
        buf->data + (buf->head * buf->datasize),
        data_size
    );
}

void ring_buffer_arena_pop(ring_buffer_arena *buf)
{
    if (buf == NULL)
    {
        return;
    }

    if (buf->size == 0)
    {
        return;
    }

    buf->head =
        (buf->head + 1) % buf->cap;

    buf->size--;
}

void ring_buffer_arena_get(
    ring_buffer_arena *buf,
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

    size_t physical_index =
        (buf->head + index) % buf->cap;

    memcpy(
        data,
        buf->data + (physical_index * buf->datasize),
        data_size
    );
}

void ring_buffer_arena_set(
    ring_buffer_arena *buf,
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

    size_t physical_index =
        (buf->head + index) % buf->cap;

    memcpy(
        buf->data + (physical_index * buf->datasize),
        data,
        data_size
    );
}

size_t ring_buffer_arena_size(ring_buffer_arena *buf)
{
    if (buf == NULL)
    {
        return 0;
    }

    return buf->size;
}

size_t ring_buffer_arena_cap(ring_buffer_arena *buf)
{
    if (buf == NULL)
    {
        return 0;
    }

    return buf->cap;
}