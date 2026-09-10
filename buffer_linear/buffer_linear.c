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

void buffer_destruir(buffer *b){
    elemento *atual = b->elemento;
    elemento *proximo;

    while (atual != NULL)
    {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    free(b);
    
}

void buffer_inserir(buffer *b, int valor){
    if(b->qtd < b->cap){
        elemento *e = malloc(sizeof(elemento));
        elemento *atual = e;

        e->valor = valor;

        e->proximo = NULL;

        b->elemento = e;

        while (atual != NULL)
        {
            proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }

        b->qtd++;
    }
}

int buffer_capacidade(buffer *b){
    return b->cap;
}

int buffer_quantidade(buffer *b){
    return b->qtd;
}
