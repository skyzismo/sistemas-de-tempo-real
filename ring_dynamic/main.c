#include <stdio.h>
#include "ring_buffer.h"

int main(void)
{
    ring_buffer *buf =
        ring_buffer_create(3, sizeof(int));

    if (buf == NULL)
    {
        printf("Erro ao criar buffer\n");
        return 1;
    }

    int a = 10;
    int b = 20;
    int c = 30;
    int d = 40;
    int resultado;

    ring_buffer_put(buf, &a, sizeof(a));
    ring_buffer_put(buf, &b, sizeof(b));
    ring_buffer_put(buf, &c, sizeof(c));

    printf("Tamanho: %zu\n", ring_buffer_size(buf));

    ring_buffer_peek(buf, &resultado, sizeof(resultado));
    printf("Peek: %d\n", resultado);

    ring_buffer_pop(buf);

    ring_buffer_put(buf, &d, sizeof(d));

    for (size_t i = 0; i < ring_buffer_size(buf); i++)
    {
        ring_buffer_get(buf, i, &resultado, sizeof(resultado));

        printf("[%zu] = %d\n", i, resultado);
    }

    ring_buffer_destroy(&buf);

    return 0;
}