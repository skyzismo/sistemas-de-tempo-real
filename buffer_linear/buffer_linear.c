#include<buffer_linear.h>
#include<stdlib.h>

typedef struct elemento
{
    int valor;  
    struct elemento *proximo;
}elemento;


typedef struct buffer
{
    elemento *elemento;
    int cap, qtd;
}buffer;

buffer *buffer_criar(int cap){
    buffer *b = malloc(sizeof(buffer)); //cria a estrutura

    if (b != NULL){
        b->elemento = NULL;
        b->cap = cap;
        b->qtd = 0;
    }

    return b; //retorna o endereço
}
