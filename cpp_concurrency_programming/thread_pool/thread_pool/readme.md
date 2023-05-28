# ThreadPool Implement 
# C version
## reference
[C -- reference](https://subingwen.cn/linux/threadpool/#5-%E6%BA%90%E6%96%87%E4%BB%B6%E5%AE%9A%E4%B9%89)

## insight
线程池中主要有如下三个组件：
1. 任务队列 task_queue
2. 工作线程 worker
3. 管理线程 manager

任务队列用来接受用户传递的“任务”，原本，用户是需要自己创建线程来执行自己的任务的，但是现在，用户不需要创建线程，它只需要将任务(函数)和任务所需要的数据(函数参数)传递到我们在线程池中提供的 API 中即可。

工作线程用来执行用户传递的任务，用户线程有多个(显然的，如果只有一个，多线程还有啥意义)。
工作线程会先进行一些检查，例如：线程池已经关闭？任务队列没空？等等，如果没有问题，它会从任务队列拿走一个线程并执行，实际上就相当于用户自己创建一个线程来执行任务，但是通过线程池管理的好处就是，这里工作线程执行完任务之后，不需要销毁，他可以继续等待下一个任务，由此来减少进程的创建和销毁带来的额外开销(这也是线程池最主要的目的，性能！)。

管理线程，顾名思义，它是用来管理工作线程的，在工作线程中提到过，工作线程在执行完毕一个用户提交的任务之后，不会销毁，那么又可能工作线程太多，而用户提交任务很少，那么就造成了很大的浪费，因此需要通过一个管理员来动态销毁线程。
同样，如果用户提交的任务太多，也可以动态添加线程。
不过，管理人的人比工作的人要高出一等(bushi)，因此，它不需要想工作线程那样无时无刻不在运行，它只需要间隔性的检查就行了。

总上，整个流程大致就是，用户提交静态的数据(函数+参数)给线程池，线程池中的工作线程负责取走任务并执行任务。管理线程负责动态维护线程池的开销。

注意，对于用户的任务不是创建一个线程来执行！而是交给工作线程，让它来执行。更不是由工作线程创建一个线程来执行。别搞混了。

---------------------------------------------

# C++ version
## reference
[github-simple-thread-pool](https://github.com/jencoldeng/ThreadPool/tree/master)

## files
cpp_thread_pool.h
cpp_main.cpp

## learn design
### 1. result_of
`result_of<>::type` 

### 2. Args...
``` C++
// std::result_of<Func(Args...)>::type; //(1)
template<typename Args... Args> //(2)
size_t f(Args&&... args) // (3)
{
    return sizeof...(args); // (4)
    // std::forward<Args>(args)...)); (5)
}
```
### 3. pass member function
use std::mem_fn() to pass a member function
``` c++
pool->push_future_task(
    std::mem_fn(&Num::summary), &n)
);
```
## insight
1. 当线程池关闭时，如果仍有任务添加到任务队列。对于单一任务，立即执行，批量任务，拒绝添加。
2. 添加单一任务时，要 notify_one()，添加批量任务时，要 notify_all()