#include"buffer_linear.h"
#include<stdlib.h>

typedef struct elemento
{
    int valor;  
    struct elemento *proximo;
}elemento;


typedef struct buffer
{
    elemento *primeiro;
    int cap, qtd;
}buffer;

buffer *buffer_criar(int cap){

    buffer *b = malloc(sizeof(buffer)); //cria a estrutura

    if (b != NULL){
        b->primeiro = NULL;
        b->cap = cap;
        b->qtd = 0;
    }

    return b; //retorna o endereço
}

void buffer_destruir(buffer *b){
    elemento *atual = b->primeiro;
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

        if(e != NULL){
            e->valor = valor;
            e->proximo = NULL; // sera o ultimo elemento

            if (b->primeiro == NULL){
                b->primeiro = e;
            }

            else{
                
                elemento *atual = b->primeiro;

                while (atual->proximo != NULL)
                {
                    atual = atual->proximo;
                }

                atual->proximo = e;
            }

            b->qtd++;
            
        }
    }
}

int buffer_remover(buffer *b){
    if (b->primeiro == NULL)
    {
        return -1;
    }

    elemento *removido = b->primeiro;
    int valor = removido->valor;

    b->primeiro = b->primeiro->proximo;

    free(removido);

    b->qtd--;

    return valor;
    
}

int buffer_capacidade(buffer *b){
    return b->cap;
}

int buffer_quantidade(buffer *b){
    return b->qtd;
}
