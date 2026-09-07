#include<stdio.h>

void alterar(int *p){
    *p = 50;
}

void alterar_dupla(int *x, int *y){
    int tmp;
    tmp = *x;
    *x = *y;
    *y = tmp;
}

int main(int argc, char const *argv[])
{
    /* code */

    int a = 10;
    int b = 20;
    int c = 30;

    int x = 10;
    int y = 20;

    int *p_a = &a;
    int *p_b = &b;
    int *p_c = &c;

    printf("%d\n", *p_a);
    printf("%p\n", p_a);
    printf("%d\n", *p_b);
    printf("%p\n", p_b);
    printf("%d\n", *p_c);
    printf("%p\n", p_c);

    alterar(&a);

    printf("%d\n", a);

    alterar_dupla(&x, &y);

    printf("%d %d\n", x, y);

    return 0;
}
