#include<stdio.h>
#include<stdbool.h>
#include<threads.h>
#include<time.h>

#define UT 20
#define MS 1000000

void generic_task(char *name, long  comp){
    thread_local static int c = 0; 
    struct timespec ts =  (struct timespec){.tv_sec = 0, .tv_nsec= comp * UT * MS};
    printf("Task %s(%d)\n", name, c);
    // c++;
    thrd_sleep(&ts, NULL);
}

typedef void (*task)(char *name ,long comp);

int main()
{
    printf("**Escalonador Cíclico**\n");

    task A = generic_task;
    task B = generic_task;
    task C = generic_task;
    task D = generic_task;
    task E = generic_task;

    task Idle = generic_task;

    while (1) //TCP
    {
        
    int next_task = 'A';
    long comp = 0;
    int tcs = 0;
    bool tcs_finished = false;
    
    while(!tcs_finished) //TCS
    {
        printf("tcs %d, next_task = %c\n", tcs, next_task);
        switch (next_task)
        {
        case 'A':
            A("A", 10);
            comp += 10;
            next_task = 'B';
            break;

        case 'B':
            B("B", 8);
            comp += 8;


            switch (tcs)
            {
            case 0:
            case 2:
                next_task = 'C';
                break;

            case 1:
            case 3:
                next_task = 'D';
                break;
            
            default:
                break;
            }

            break;

        case 'C':
            C("C", 5);
            comp += 5;
            if(!(tcs == 0 || tcs ==2 )) printf("Erro no ciclo em C\n");
            next_task = 'I';
            break;

        case 'D':
            D("D", 4);
            comp += 4;

            switch (tcs)
            {
            case 1:
                next_task = 'E';
                break;

            case 3:
                next_task = 'I';
                break;
            
            default:
                printf("Erro no ciclo\n");
                next_task = 'I';
                break;
            }
            
            break;

        case 'E':
            E("E", 2);
            comp += 2;

            if (tcs != 1) printf("Erro no ciclo em E\n");
            next_task = 'I';
            break;
        
        default:

            Idle("Idle", 25-comp);
            next_task = 'A';
            tcs++;
            tcs_finished = (tcs ==4) ? true : false;
            break;
        }

        printf("tcs %d, next_task = %c\n", tcs, next_task);
    }
    }
    
    

    printf("**Tchau!**\n");
    return 0;
}
