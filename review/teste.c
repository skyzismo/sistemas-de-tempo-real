#include<stdio.h>
#include<stdlib.h>

typedef struct elemento
{   
    /* data */

    int valor;
    struct elemento *proximo;
}elemento;   


int main(int argc, char const *argv[])
{
    elemento *no1 = malloc(sizeof(elemento));
    elemento *no2 = malloc(sizeof(elemento));
    elemento *no3 = malloc(sizeof(elemento));
    elemento *no4 = malloc(sizeof(elemento));

    no1->valor = 10;
    no1->proximo = no2;

    no2->valor = 20;
    no2->proximo = no3;

    no3->valor = 30;
    no3->proximo = no4;

    no4->valor = 40;
    no4->proximo = NULL;

    elemento *atual = no1;

    while (atual != NULL)
    {
        printf("%d\n", atual->valor);
        atual = atual->proximo;
    }

    free(no1);
    free(no2);
    free(no3);
    free(no4);

    return 0;
}
