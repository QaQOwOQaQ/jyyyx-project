#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <thread_pool.h>


// max count of threads the manager can add or destroy every time
const int NUMBER = 2;

typedef enum boolean_t boolean;
typedef struct task_t Task;
typedef struct thread_pool_t ThreadPool;

static void errorMessage(const char *s);
ThreadPool* threadPoolCreate(int minNum, int maxNum, int capacity);
boolean threadPoolDestroy(ThreadPool *pool);
void threadPoolAdd(ThreadPool *pool, void(*func)(void*), void *arg);
int threadPoolBusyNum(ThreadPool *pool);
int threadPoolAliveNum(ThreadPool *pool);
void* worker(void *arg);
void* manager(void *arg);
void threadExit(ThreadPool *pool);

static void errorMessage(const char *s)
{
    printf("[Error] %s\n", s);
    // exit(0);
}

ThreadPool* threadPoolCreate(int minNum, int maxNum, int capacity)
{
    // create thread pool
    ThreadPool *pool = (ThreadPool*)malloc(sizeof(ThreadPool));
    // make the program execute only omnce by do-while(0)
    do {
        if(pool == NULL)
        {
            errorMessage("malloc thread pool failed");
            break;
        }

        // init mutex and cond
        if(pthread_mutex_init(&pool->mutexBusy, NULL) || 
            pthread_mutex_init(&pool->mutexPool, NULL) || 
            pthread_cond_init(&pool->notEmpty, NULL) || 
            pthread_cond_init(&pool->notFull, NULL))
        {
            errorMessage("mutex or cond init fault");
            break;
        }

        // init work threads
        pool->threadIDs = (pthread_t*)malloc(sizeof(pthread_t) * maxNum);
        if(pool->threadIDs == NULL)
        {
            errorMessage("malloc threadIDs failder");
            break;
        }
        memset(pool->threadIDs, 0, sizeof(pthread_t) * maxNum);
        pool->minNum = minNum;
        pool->maxNum = maxNum;
        pool->busyNum = 0;
        pool->liveNum = minNum; // 😭equal to minNum (start at minx to save memory)
        pool->exitNum = 0;
        // create pthread
            /*  when we call pthread_create() to create a thread
                the thread wil execute immediately(?) until we exit it or it sucide
                ...
                so now, minNum workers and one manager are runnng ~~~
                ...
                manager thread will running in the time through thread pool live
                    -- it use while() and sleep() to interval running ~~ 
                ...
                worker thread do the "work thread" actually
                the function and arg which user passed is just a expection
                (you can get it because the message which user passed is just a struct of task
                so it is a static message, worker thread do it activity)
                the worker thread call "task.fptr(task.arg)" to execute the task which user passed actually
            */
        pthread_create(&pool->managerId, NULL, manager, pool);
        for(int i = 0; i < minNum; i ++ )
        {
            pthread_create(&pool->threadIDs[i], NULL, worker, pool);
        }

        // init task queue
        pool->taskQ = (Task*)malloc(sizeof(Task) *capacity);
        if(pool->taskQ == NULL)
        {
            errorMessage("malloc taskQ fault");
            break;
        }
        pool->queueCapacity = capacity;
        pool->queueSize = 0;
        pool->queueFront = 0;
        pool->queueRear = 0;   

        // init shutdown
        pool->shutdown = FALSE;

        // success create thread pool
#ifdef DEBUG
        printf("create thread pool successful!\n");
#endif 
        return pool;
    } while(0);
    
    // it means that do-while(0) failed if program go here
    // so we must to free the memory we have malloced
    // we needn't to free the resources like mutex and conf
    //    -- because it will managed by os, we only need concern about the memory we malloced in heap
    if(pool && pool->threadIDs) free(pool->threadIDs);
    if(pool && pool->taskQ)     free(pool->taskQ);
    if(pool)        free(pool); // should at the ending
    return NULL;
}

boolean threadPoolDestroy(ThreadPool *pool)
{
    // pool is NULL
    if(pool == NULL)    
    {
        return FALSE;
    }

    // close thread pool
    pool->shutdown = TRUE;
    // block and recycle(回收) manager thread
    pthread_join(pool->managerId, NULL);
    // wake the blocked consumer thread
    for(int i = 0; i < pool->liveNum; i ++ )
    {
        pthread_cond_signal(&pool->notEmpty);
    }
    // free heap memory
    if(pool->taskQ)
    {
        free(pool->taskQ);
    }
    if(pool->threadIDs)
    {
        free(pool->threadIDs);
    }

    // destroy mutex and cond
    pthread_mutex_destroy(&pool->mutexPool);
    pthread_mutex_destroy(&pool->mutexBusy);
    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);

    // free thread pool
    free(pool);
    pool = NULL;
#ifdef DEBUG
    printf("thread pool destroy successful!\n");
#endif 
    return TRUE;
}

void threadPoolAdd(ThreadPool *pool, void(*func)(void*), void *arg)
{
    pthread_mutex_lock(&pool->mutexPool);
    // thread pool has been shutdown, can't add new task
    if(pool->shutdown)
    {
        // unlock mutex 
        pthread_mutex_unlock(&pool->mutexPool);
        return ;
    }
    
    // use while but if, for example:
    // in this case, there are three threads blocked for a resource A
    // and in the future, a thread free A, but just only one 
    // so only one thread can get A, and else are blocked too
    // so, you should use while
    // because although notFull signaled, this thread will blocked also
    while(pool->queueSize == pool->queueCapacity)
    {
        // block producer thread to wait not full
        pthread_cond_wait(&pool->notFull, &pool->mutexPool);
    }
    
    // add task to cycle-queue, tail in and head out
    pool->taskQ[pool->queueRear].fptr = func;
    pool->taskQ[pool->queueRear].arg = arg;
    pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;  
    pool->queueSize ++ ;

    pthread_cond_signal(&pool->notEmpty);
    pthread_mutex_unlock(&pool->mutexPool);
}

int threadPoolBusyNum(ThreadPool *pool)
{
    // can't return pool->busNum directly
    // because it will currency wrong!
    // so, you should lock first and then get value in tmp variable, ending with unlock
    pthread_mutex_lock(&pool->mutexBusy);
    int busyNum = pool->busyNum;
    pthread_mutex_unlock(&pool->mutexBusy);
    return busyNum;
}

int threadPoolAliveNum(ThreadPool *pool)
{
    pthread_mutex_lock(&pool->mutexPool);
    int avileNum = pool->liveNum;
    pthread_mutex_unlock(&pool->mutexPool);
    return avileNum;
}

// if we want to destrou a thread
// we have to accomlish it by thread pool
void threadExit(ThreadPool *pool)
{
    pthread_t tid = pthread_self();
    for(int i = 0; i < pool->maxNum; i ++ )
    {
        if(pool->threadIDs[i] == tid)
        {
            pool->threadIDs[i] = 0; // clear so we can reuse it 
            printf("threadExit() called, %ld exiting...", tid);
            break;
        }
    }
    pthread_exit(NULL); // really exit in here 😊
}

void* worker(void *arg)
{
    ThreadPool *pool = (ThreadPool*)arg;
    while(1)
    {
        pthread_mutex_lock(&pool->mutexPool);
        if(pool->shutdown)
        {
            pthread_mutex_unlock(&pool->mutexPool);
            threadExit(pool);   // call warpped function
        }
        
        while(!pool->queueSize)
        {
            // block work thread
            pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);
            // check if want to destroy thread
            if(pool->exitNum > 0)
            {
                pool->exitNum -- ;
                if(pool->liveNum > pool->minNum)
                {
                    pool->liveNum -- ;
                    pthread_mutex_unlock(&pool->mutexPool);
                    threadExit(pool);   // from this thread to exit
                }
            }
        }

        // get a task from task queue
        Task task;
        task.fptr = pool->taskQ[pool->queueFront].fptr;
        task.arg = pool->taskQ[pool->queueFront].arg;
        // move queue head 
        pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity;
        pool->queueSize -- ;
        // unlock
        pthread_cond_signal(&pool->notFull);
        pthread_mutex_unlock(&pool->mutexPool);

        // maintain thread pool
            // add busyNuM
        printf("thread 0x%x start working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum ++ ;
        pthread_mutex_unlock(&pool->mutexBusy);
            // begin execute
        task.fptr(task.arg);
            // if go ther, means fpre() have been exected down
            // so we should release resources and reduce busyNum
            // after execution, release arg which is malloced in heap 
        free(task.arg);
        task.arg = NULL;    // set NULL if after free

        printf("thread 0x%x end working...\n", pthread_self());
        pthread_mutex_lock(&pool->mutexBusy);
        pool->busyNum -- ;  // exection down
        pthread_mutex_unlock(&pool->mutexBusy);
    }
    return NULL;
}

// monitor thread pool resources and dynamically adjust(add or reduce threads)
void* manager(void *arg)
{
    ThreadPool *pool = (ThreadPool*) arg;
    while(!pool->shutdown)
    {
        // check every three seconds
        sleep(3);

            /* First, statistical resources */
        // get the count of tasks and threads in current
        pthread_mutex_lock(&pool->mutexPool);
        int queueSize = pool->queueSize;    // tasks
        int liveNum = pool->liveNum;        // threads
        pthread_mutex_unlock(&pool->mutexPool);

        // get the count of threads which is busy
        pthread_mutex_lock(&pool->mutexBusy);
        int busyNum = pool->busyNum;
        pthread_mutex_unlock(&pool->mutexBusy);

            /* Second, to add thread */
        // when cnt of tasks > cnt of alive threads 
        // and  cnt of alive threads < cnt of max threads(so we can add)
        if(queueSize > liveNum && liveNum < pool->maxNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            int counter = 0;
            // travel all threads to get a idled thread
            for(int i = 0; i < pool->maxNum && 
                counter < NUMBER && 
                pool->liveNum < pool->maxNum; 
                i ++ )
            {
                if(pool->threadIDs[i] == 0) // not alive
                {
                    pthread_create(&pool->threadIDs[i], NULL, worker, pool);
                    counter ++ ;
                    pool->liveNum ++ ;
                }
            }
            pthread_mutex_unlock(&pool->mutexPool);
        }
        
            /* Second, to destroy thread */
        // we take a specific way to check if we need to destroy thread
        // when cnt of busy threads < alive threads (experience way, not best way...)
        // and  alive threads > cnt of min threads (so we can destory)
        if(busyNum * 2 < liveNum && liveNum > pool->minNum)
        {
            pthread_mutex_lock(&pool->mutexPool);
            pool->exitNum = NUMBER;
            pthread_mutex_unlock(&pool->mutexPool);
            // add the count of exitNum to make threads suicide when they are idled(空闲的)
            // take about it, where are the idle threads?
            // through the worker function, we can found that the threads which is not work is blocked by cond
            // if we want to make it suicide, wo must wake it first
            // so we can signal that cond, and provide some check(exitNum) in worker function
            // enable threads to check if they should kill thmeselves
            for(int i = 0; i < NUMBER; i ++ )
            {
                pthread_cond_signal(&pool->notEmpty);
            }
        }
    }
    return NULL;
}