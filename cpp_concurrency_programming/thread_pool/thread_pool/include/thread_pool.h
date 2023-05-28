#ifndef THREAD_POOL_C_GUARD

#include <pthread.h>

/* ========== */
/*    Bool    */
/* ========== */
typedef enum boolean_t {
    FALSE, TRUE
} boolean;

/*==========================*/
/*   Data struct of task    */
/*==========================*/
typedef void(*fptr_t)(void*);

typedef struct task_t {
    fptr_t fptr;        // function ptr
    void *arg;          // arguments
} Task;

/*================================*/
/*   Data struct of thread pool   */
/*================================*/
typedef struct thread_pool_t {
    Task *taskQ;        // task queue
    int queueCapacity;  // the capacity of task queue (equal to max size)
    int queueSize;      // cur count of tasks
    int queueFront;     // task queue head -> get data
    int queueRear;      // task queue tail -> put data

    pthread_t managerId;    // manager thread id
    pthread_t *threadIDs;   // work threads id
    int minNum;             // the maximum numbre of work threads
    int maxNum;             // the minimum number of work threads
    int busyNum;            // working threads now
    int liveNum;            // living threads now(contain working threads and free threads)
    int exitNum;            /* if living threads more than working threads too much
                             delete this number threads */
    
    pthread_mutex_t mutexPool;  // provide synchoronization for all thread pool
    pthread_mutex_t mutexBusy;   // special mutex for busy number, because it would changed very frequently

    boolean shutdown;           // 1 to destroy thread pool, else 0
    pthread_cond_t notFull;     // is task queue is full
    pthread_cond_t notEmpty;    // is task queue is empty
} ThreadPool;

// create and initialization thread pool
ThreadPool* threadPoolCreate(int minNum, int maxNum, int capacity);

// destroy thread pool
boolean threadPoolDestroy(ThreadPool *pool);

// add task to thread pool
void threadPoolAdd(ThreadPool *pool, void(*func)(void*), void *arg);

// get the count of working threads
int threadPoolBusyNum(ThreadPool *pool);

// get the count of living threads
int threadPoolAliveNum(ThreadPool *pool);

// work threads' task function
void* worker(void *arg);

// manager thread's task function
void* manager(void *arg);

// single thread exit
void threadExit(ThreadPool *pool);

#define THREAD_POOL_C_GUARD
#endif 