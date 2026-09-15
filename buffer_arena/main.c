#include <stdio.h>
#include "arena.h"
#include "buffer_arena.h"

int main(void)
{
    arena *a = arena_create(1024);

    if (a == NULL)
    {
        printf("Erro ao criar arena\n");
        return 1;
    }

    buffer_arena *buf =
        buffer_arena_create(a, 5, sizeof(int));

    if (buf == NULL)
    {
        printf("Erro ao criar buffer\n");
        arena_destroy(&a);
        return 1;
    }

    int valor = 10;
    int resultado;

    buffer_arena_put(buf, &valor, sizeof(valor));

    buffer_arena_get(
        buf,
        0,
        &resultado,
        sizeof(resultado)
    );

    printf("Resultado: %d\n", resultado);

    printf("Buffer: %zu/%zu\n",
           buffer_arena_size(buf),
           buffer_arena_cap(buf));

    printf("Arena: %zu/%zu bytes\n",
           arena_used(a),
           arena_cap(a));

    /*
     * Não destruímos o buffer individualmente.
     * A Arena é responsável pela memória.
     */

    arena_destroy(&a);

    return 0;
}