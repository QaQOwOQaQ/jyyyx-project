#include <stdio.h>
#include <unistd.h>

int gval = 10;

int main()
{
    pid_t pid;
    int lval = 20;
    gval ++ , lval += 5;    
    // gval = 11, lval = 25
    pid = fork();
    if(pid == 0)
        gval += 2, lval += 2;
        // gval = 13, lval = 27
    else 
        gval -= 2, lval -= 2;
        // gval = 9, lval = 23
    if(pid == 0)
        printf("child %d: %d, %d\n", getpid(), lval, gval);
    else
        printf("father %d - %d: %d, %d\n", getpid(), pid, lval, gval);
    return 0;
}