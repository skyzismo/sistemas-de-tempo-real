#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

typedef struct buffer buffer;

buffer *buffer_create(size_t size, size_t datasize);
void buffer_destroy(buffer **pbuf);

void buffer_put(buffer *buf, void *data, size_t data_size);
void buffer_peek(buffer *buf, void *data, size_t data_size);

void buffer_get(buffer *buf, size_t index, void *data, size_t data_size);
void buffer_set(buffer *buf, size_t index, void *data, size_t data_size);

size_t buffer_size(buffer *buf);
size_t buffer_cap(buffer *buf);

#endif