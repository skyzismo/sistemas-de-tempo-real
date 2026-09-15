#include "buffer_linear.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    buffer *b = buffer_criar(3);

    printf("capacidade: %d\n", buffer_capacidade(b));
    printf("quantidade: %d\n", buffer_quantidade(b));

    buffer_inserir(b, 10);
    buffer_inserir(b, 20);
    buffer_inserir(b, 30);
    buffer_inserir(b, 40);

    printf("capacidade: %d\n", buffer_capacidade(b));
    printf("quantidade: %d\n", buffer_quantidade(b));

    printf("removido: %d\n", buffer_remover(b));
    printf("removido: %d\n", buffer_remover(b));

    buffer_inserir(b, 40);

    printf("removido: %d\n", buffer_remover(b));
    printf("removido: %d\n", buffer_remover(b));
    printf("removido: %d\n", buffer_remover(b));

    printf("%d\n", buffer_capacidade(b));
    printf("%d\n", buffer_quantidade(b));

    buffer_destruir(b);

}
