#include<stdio.h>

int main(int argc, char const *argv[])
{
    /* code */

    int numeros[5] = {10, 20, 30, 40, 50};

    int *p = &numeros[0];
    
    printf("%d\n", p[0]);

    printf("%d\n", p[1]);

    printf("%d\n", p[2]);

    printf("%d\n", p[3]);

    printf("%d\n", p[4]);

    p[0] = 100;
    p[1] = 200;
    p[2] = 300;
    p[3] = 400;
    p[4] = 500;

    printf("%d %d %d %d %d\n", numeros[0], numeros[1],numeros[2],numeros[3],numeros[4]);
    return 0;
}
