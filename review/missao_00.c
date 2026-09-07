#include <stdio.h>
#include <string.h>

int soma(int a, int b){
    return a + b;
}

int main(int argc, char const *argv[])
{
    /* code */

    int idade, numero;
    float altura;
    char inicial;

    idade = 24;
    numero = 22;
    inicial = 'L';
    altura = 1.73;

    printf("%d\n", idade);
    printf("%d\n", numero);
    printf("%c\n", inicial);
    printf("%.2f\n", altura);

    idade = 25;

    printf("%d\n", numero*2);
    printf("%d\n", idade);

    int r_soma = soma(4,5);
    printf("%d\n", r_soma);

    printf("\nOutra parada agora, fi\n");

    numero = 42;

    int *ponteiro = &numero;

    printf("%d\n", numero);
    printf("%p\n", &numero);
    printf("%d\n", *ponteiro);
    printf("%p\n", ponteiro);

    *ponteiro = 100;

    printf("%d\n", numero);

    

    return 0;
}
