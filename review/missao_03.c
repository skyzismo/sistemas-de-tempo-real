#include<stdio.h>

int main(int argc, char const *argv[])
{
    /* code */

    int numeros[5] = {10, 20, 30, 40, 50};

    int *p = &numeros[0];

    printf("%d\n", *p);
    p++;
    printf("%d\n", *p);
    p++;
    printf("%d\n", *p);
    p++;
    printf("%d\n", *p);
    p++;
    printf("%d\n", *p);
    return 0;
}
