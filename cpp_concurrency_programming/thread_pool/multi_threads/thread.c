#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#define COUNT (5)

sem_t psem; // producer
sem_t csem; // comsumer
pthread_mutex_t mutex;

typedef struct node_t {
    int val;
    struct node_t *next;
} Node;

Node *head;

void* produce(void *arg)
{
    while(1)
    {
        sem_wait(&psem);
        // pthread_mutex_lock(&mutex); // if pshared = 1, can omission
        Node *newNode = (Node*)malloc(sizeof(Node));
        newNode->val = rand() % 1000;
        newNode->next = NULL;
        newNode->next = head;
        head = newNode;
        printf("[Producer] id = %lu, val = %d\n", pthread_self(), newNode->val);
        // pthread_mutex_unlock(&mutex);
        sem_post(&csem);
        sleep(rand() % 5);
    }
    return NULL;
}

void* consume(void *arg)
{
    while(1)
    {
        sem_wait(&csem);
        // pthread_mutex_lock(&mutex);
        Node *curNode = head;
        head = head->next;
        printf("[Consumer] id = %lu, val = %d\n", pthread_self(), curNode->val);
        free(curNode);
        // pthread_mutex_unlock(&mutex);
        sem_post(&psem);
        sleep(rand() % 5);
    }
    return NULL;
}

int main()
{
    srand(time(NULL));
    pthread_mutex_init(&mutex, NULL);
    sem_init(&psem, 0, 5);
    sem_init(&csem, 0, 0);
    pthread_t t1[COUNT], t2[COUNT];
    for(int i = 0; i < COUNT; i ++ )
    {
        pthread_create(&t1[i], NULL, produce, NULL);
        pthread_create(&t2[i], NULL, consume, NULL);
    }
    for(int i = 0; i < COUNT; i ++ )
    {
        pthread_join(t1[i], NULL);
    }
    for(int i = 0; i < COUNT; i ++ )
    {
        pthread_join(t2[i], NULL);
    }
    return 0;
}
// 75 -> 852 -> 54