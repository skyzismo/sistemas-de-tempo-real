#include "ring_buffer.h"
#include <stdlib.h>
#include <string.h>

struct ring_buffer
{
    unsigned char *data;

    size_t size;
    size_t cap;
    size_t datasize;

    size_t head;
    size_t tail;
};

ring_buffer *ring_buffer_create(size_t capacity, size_t datasize)
{
    if (capacity == 0 || datasize == 0)
    {
        return NULL;
    }

    ring_buffer *buf = malloc(sizeof(ring_buffer));

    if (buf == NULL)
    {
        return NULL;
    }

    buf->data = malloc(capacity * datasize);

    if (buf->data == NULL)
    {
        free(buf);
        return NULL;
    }

    buf->size = 0;
    buf->cap = capacity;
    buf->datasize = datasize;

    buf->head = 0;
    buf->tail = 0;

    return buf;
}

void ring_buffer_destroy(ring_buffer **pbuf)
{
    if (pbuf == NULL || *pbuf == NULL)
    {
        return;
    }

    ring_buffer *buf = *pbuf;

    free(buf->data);
    free(buf);

    *pbuf = NULL;
}

void ring_buffer_put(ring_buffer *buf, void *data, size_t data_size)
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

    buf->tail = (buf->tail + 1) % buf->cap;
    buf->size++;
}

void ring_buffer_peek(ring_buffer *buf, void *data, size_t data_size)
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

void ring_buffer_pop(ring_buffer *buf)
{
    if (buf == NULL)
    {
        return;
    }

    if (buf->size == 0)
    {
        return;
    }

    buf->head = (buf->head + 1) % buf->cap;
    buf->size--;
}

void ring_buffer_get(
    ring_buffer *buf,
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

void ring_buffer_set(
    ring_buffer *buf,
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

size_t ring_buffer_size(ring_buffer *buf)
{
    if (buf == NULL)
    {
        return 0;
    }

    return buf->size;
}

size_t ring_buffer_cap(ring_buffer *buf)
{
    if (buf == NULL)
    {
        return 0;
    }

    return buf->cap;
}