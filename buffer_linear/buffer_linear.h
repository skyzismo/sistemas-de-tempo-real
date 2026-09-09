#ifndef BUFFER_LINEAR_H
#define BUFFER_LINEAR_H

typedef struct buffer buffer;

buffer *buffer_criar(int cap);
void *buffer_destruir(buffer *b);

int buffer_capacidade(buffer *b);
int buffer_quantidade(buffer *b);

#endif