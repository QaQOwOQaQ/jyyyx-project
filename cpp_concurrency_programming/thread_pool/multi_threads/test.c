#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void add(int *restrict p)
{
     ++ *p;
    int *q = (int*)p;
    ++ *q;
}

int main()
{
    int x = 1;
    add(&x);
    printf("%d\n", x);


    return 0;
}