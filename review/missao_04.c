#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[])
{
    /* code */

    int quantidade = 5;

    int *p = malloc(quantidade * sizeof(int));

    if(p != NULL){
        p[0] = 10;
        p[1] = 20;
        p[2] = 30;
        p[3] = 40;
        p[4] = 50;

        for (int i = 0; i < 5; i++){
            printf("%d\n", p[i]);
        }

        free(p);
    }

    return 0;
}
