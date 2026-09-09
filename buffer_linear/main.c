#include "buffer_linear.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    buffer *b = buffer_criar(5);

    printf("%d\n", buffer_capacidade(b));
    printf("%d\n", buffer_quantidade(b));

    buffer_destruir(b);

    printf("%d\n", buffer_capacidade(b));
    printf("%d\n", buffer_quantidade(b));

}
