#include <stdio.h>
#include "buffer.h"

int main(void)
{
    buffer *buf = buffer_create(5, sizeof(int));

    if (buf == NULL)
    {
        printf("Erro ao criar buffer\n");
        return 1;
    }

    int a = 10;
    int b = 20;
    int c = 30;
    int resultado;

    buffer_put(buf, &a, sizeof(a));
    buffer_put(buf, &b, sizeof(b));
    buffer_put(buf, &c, sizeof(c));

    printf("Tamanho: %zu\n", buffer_size(buf));
    printf("Capacidade: %zu\n", buffer_cap(buf));

    buffer_get(buf, 1, &resultado, sizeof(resultado));

    printf("Elemento 1: %d\n", resultado);

    int novo = 99;

    buffer_set(buf, 1, &novo, sizeof(novo));

    buffer_get(buf, 1, &resultado, sizeof(resultado));

    printf("Elemento 1 depois do set: %d\n", resultado);

    buffer_peek(buf, &resultado, sizeof(resultado));

    printf("Peek: %d\n", resultado);

    buffer_destroy(&buf);

    return 0;
}