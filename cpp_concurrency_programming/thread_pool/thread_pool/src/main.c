#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <thread_pool.h>

void taskFunc(void* arg)
{
    int num = *(int*)arg;
    printf("thread 0x%x is working, number = 0x%x\n",
        pthread_self(), num);
    sleep(1);
}

int main()
{
    // 创建线程池
    ThreadPool* pool = threadPoolCreate(10, 20, 100);
    for (int i = 0; i < 100; ++i)
    {
        int* num = (int*)malloc(sizeof(int));
        *num = i + 100;
        threadPoolAdd(pool, taskFunc, num);
    }

    sleep(10);

    threadPoolDestroy(pool);
    return 0;
}