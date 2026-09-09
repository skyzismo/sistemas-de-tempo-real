#include"buffer_linear.h"
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

void *buffer_destruir(buffer *b){
    elemento *atual = b->elemento;

    while (atual != NULL)
    {
        atual = atual->proximo;
        free(atual);
        atual = atual->valor;
    }

    free(b);
    
}

int buffer_capacidade(buffer *b){
    return b->cap;
}

int buffer_quantidade(buffer *b){
    return b->qtd;
}
