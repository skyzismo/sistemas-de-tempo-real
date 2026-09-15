#include "buffer.h"
#include <stdlib.h>
#include <string.h>

struct buffer
{
    unsigned char *data;
    size_t size;
    size_t cap;
    size_t datasize;
};

buffer *buffer_create(size_t size, size_t datasize)
{
    if (size == 0 || datasize == 0)
    {
        return NULL;
    }

    buffer *buf = malloc(sizeof(buffer));

    if (buf == NULL)
    {
        return NULL;
    }

    buf->data = malloc(size * datasize);

    if (buf->data == NULL)
    {
        free(buf);
        return NULL;
    }

    buf->size = 0;
    buf->cap = size;
    buf->datasize = datasize;

    return buf;
}

void buffer_destroy(buffer **pbuf)
{
    if (pbuf == NULL || *pbuf == NULL)
    {
        return;
    }

    buffer *buf = *pbuf;

    free(buf->data);
    free(buf);

    *pbuf = NULL;
}

void buffer_put(buffer *buf, void *data, size_t data_size)
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

void buffer_peek(buffer *buf, void *data, size_t data_size)
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

void buffer_get(buffer *buf, size_t index, void *data, size_t data_size)
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

void buffer_set(buffer *buf, size_t index, void *data, size_t data_size)
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

size_t buffer_size(buffer *buf)
{
    if (buf == NULL)
    {
        return 0;
    }

    return buf->size;
}

size_t buffer_cap(buffer *buf)
{
    if (buf == NULL)
    {
        return 0;
    }

    return buf->cap;
}