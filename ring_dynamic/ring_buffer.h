#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>

typedef struct ring_buffer ring_buffer;

ring_buffer *ring_buffer_create(size_t capacity, size_t datasize);
void ring_buffer_destroy(ring_buffer **pbuf);

void ring_buffer_put(ring_buffer *buf, void *data, size_t data_size);
void ring_buffer_peek(ring_buffer *buf, void *data, size_t data_size);
void ring_buffer_get(ring_buffer *buf, size_t index, void *data, size_t data_size);
void ring_buffer_set(ring_buffer *buf, size_t index, void *data, size_t data_size);

void ring_buffer_pop(ring_buffer *buf);

size_t ring_buffer_size(ring_buffer *buf);
size_t ring_buffer_cap(ring_buffer *buf);

#endif