#ifndef CPP_THREAD_POOL_GUARD
#define CPP_THREAD_POOL_GURAD

/* README HERE FIRST 😊 
* just as you see, I add 😊 to splash that the 
* block of code or comment is considerable important ~~ 
* @copy right QAQ
* [ref here](https://github.com/jencoldeng/ThreadPool/blob/master/thread_pool.h) 
*/

/* [TODO] add manager thread
*/

#include <my_header.h>

class ThreadPool
{
public:
    typedef std::function<void()> Task;

public:
    ThreadPool(int/*threads*/);
    ~ThreadPool();
    
    /* My change 😊
    * the author set the function of copy ctor and operator= in private
    * why? it will get link error when running but call the deleted function 
    * when user call those two function, then I move those to the public privillage
    * so it will get CE when compile early and set in public .. whatever where ~~
    */
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

public:
    void stop();
    bool is_stopped();

    // get the number of waiting tasks except timed task
    int pending() const ;

    // add task 😊
    template<typename Func, typename... Args>
            // 😊 [CANT] Args... &&args and why?
    void push_task(Func &&f, Args&&... args);

    // add batch tasks 😊
    // return false if cant't add
    template<typename TaskContainer>
    bool push_batch_task(TaskContainer &&task_list);

    // add task, caller will wait until finished 😊
    template<typename Func, typename... Args>
    auto push_future_task(Func &&f, Args&&... args) 
        -> std::future<typename std::result_of<Func(Args...)>::type>;
    
    // add priority task
    template<typename Func, typename... Args>
    void push_priority_task(Func &&f, Args&&... args);

    // add delay task
    template<typename Func, typename... Args>
    void push_delay_task(int64_t delay_ms, Func &&f, Args&&... args);

    // batch add delay tasks
    template<typename TaskContainer>
    bool push_batch_delay_task(int64_t delay_ms, TaskContainer &&task_list);

private:
    // cout the time span since epoch in ms
    static int64_t Milliseconds()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

private:
    // represent a delay task
    class TimeTask
    {
    // father class wouldn't access TimeTask's data member if set private here
    public:
        int64_t exec_tm; // expected execute time
        Task task;

    public:
        TimeTask(int64_t _exec_tm, Task&& _task)
        : exec_tm(_exec_tm), task(std::move(_task)) {}
        
        TimeTask(const TimeTask& t)
        : exec_tm(t.exec_tm), task(t.task) {}
        
        // 😊 also add right reference to matche left reference
        TimeTask(const TimeTask&& t) 
        : exec_tm(t.exec_tm), task(std::move(t.task)) {}

        TimeTask& operator=(TimeTask &&t)
        {
            exec_tm = t.exec_tm;
            task = std::move(t.task);
            return *this;
        }

        // 😊 because we will push delay task in priority_queue
        // and we want queue top have the minimum exec_tm 
        bool operator<(const TimeTask &it) const 
        {
            return exec_tm > it.exec_tm;
        }
    };
private:
    mutable std::mutex queue_mutex;
    std::condition_variable queue_condition;
    std::deque<Task> tasks; // queue of tasks
    std::vector<std::thread> workers; // manage thread
    std::priority_queue<TimeTask> delay_tasks; // queue of delay task
    std::atomic_bool stop_flag{false}; // flag of quit
};

/*=========================================================================================*/
inline ThreadPool::ThreadPool(int threads)
{
    // init thread callbabk function(worker thread)
    for(int i = 0; i < threads; i ++ )
    {
        workers.emplace_back([this]{
            for(;;)
            {
                Task task;
                std::unique_lock<std::mutex> lock(queue_mutex);
                // atomic::load() return the current value of atomic variable
                queue_condition.wait(lock, [this]{
                    // wait until the following prediction satisfy
                    //      -- until have a task or stop the thread pool
                    return stop_flag.load() || !tasks.empty() || !delay_tasks.empty();
                });

                // Although the thread pool is stopped, do the task pushed before
                if(stop_flag.load() && tasks.empty())   return ;
                
                // do delay task
                if(!delay_tasks.empty())
                {
                    auto now_tm = ThreadPool::Milliseconds();
                    auto wait_tm = delay_tasks.top().exec_tm - now_tm;
                    if(wait_tm <= 0) // the current time have passed the executed time which user wanted
                                     // ececuted this task immediately
                    {
                        // get and pop
                        task = delay_tasks.top().task; // why not move() ? 😊
                        delay_tasks.pop();
                        // execute(need't to lock when execute a task)
                        lock.unlock();
                        if(task) task();    // why check ? 😊
                        lock.lock();
                    }
                    // if not emergency delay task and no normal task
                    // please just wait a meantime
                    else if(tasks.empty() && !stop_flag.load())
                    {
                        queue_condition.wait_for(lock, std::chrono::milliseconds(wait_tm));
                    }
                }
                
                // do normal task
                if(!tasks.empty()) 
                {
                    // get and pop
                    task = std::move(tasks.front());
                    tasks.pop_front();
                    // execute
                    lock.unlock();
                    if(task) task(); // why check ? 😊
                    lock.lock();
                }
            } // for end
        }  /*lambda end*/ ); // emplace_back end
    }
}

// stop ThreadPool
inline ThreadPool::~ThreadPool()
{
    this->stop();
}

// stop and wait thread finished
inline void ThreadPool::stop()
{
    //  use local scope {} to release unique_lock early
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        // atomic::store() will replace the current value
        stop_flag.store(true);
        // wake all worker thread to complete the remained task
        queue_condition.notify_all();
    }
    for(std::thread &worker : workers)
    {
        if(worker.joinable())
            worker.join();
    }   
}

inline bool ThreadPool::is_stopped()
{
    return stop_flag.load();
}

template<typename Func, typename... Args>
void ThreadPool::push_task(Func &&f, Args&&... args)
{
    std::unique_lock<std::mutex> lock(queue_mutex);

    // if threadPool has been stoped, do task imediatelly
    if(stop_flag.load()) 
    {
        lock.unlock();
        f(args...);
    }
    // else push the task to the tasks_queue
    else 
    {
        /* typedef std::function<void()> Task;
         * because typedef Task do not have parameter
         * so have to bind arguments to the task we passed
         */
        if(sizeof...(args))
            tasks.emplace_back(std::bind(std::forward<Func>(f), std::forward<Args>(args)...));
        else 
            tasks.emplace_back(std::move(f));
        queue_condition.notify_one();
    }
}

// return false if cant't add
template<class TaskContainer>
bool ThreadPool::push_batch_task(TaskContainer &&task_list)
{
    if(task_list.empty())   return false;
    /* here I chane the sequence of check stop and lock
    * because I think it's needn't to lock when access stop_flag 
    */
    if(stop_flag.load())    return false;

    std::unique_lock<std::mutex> lock(queue_mutex);
    for(auto &task : task_list)
        tasks.emplace_back(std::move(task));
    task_list.clear();  // clear user's task queue
    // becuause we add a bitch of tasks
    // so notify all here but notice one
    queue_condition.notify_all();
    return true;
}

template<typename Func, typename... Args>
auto ThreadPool::push_future_task(Func &&f, Args&&... args)
    -> std::future<typename std::result_of<Func(Args...)>::type>
{
    using return_type = typename std::result_of<Func(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        // std::forward<<Args>(args)...  😊 the position of ...
        std::bind(std::forward<Func>(f), std::forward<Args>(args)...)
    );
    std::future<return_type> res = task->get_future();

    std::unique_lock<std::mutex> lock(queue_mutex);
    if(stop_flag.load()) // execute immediately if thread pool is stooped
    {
        lock.unlock();
        (*task)();
    }
    else 
    {
        tasks.emplace_back([task]{ (*task)(); }); // ??? 😊
        queue_condition.notify_one();
    }
    return res;
}

// normal task push back and priority task push front
template<typename Func, typename... Args>
void ThreadPool::push_priority_task(Func &&f, Args&&... args)
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    if(stop_flag.load())
    {
        lock.unlock();
        f(args...);
    }
    else 
    {
        if(sizeof...(args))
            tasks.emplace_front(std::bind(std::forward<Func>(f), std::forward<Args>(args)...));
        else 
            tasks.emplace_front(std::move(f));
        queue_condition.notify_one();
    }
}

template<typename Func, typename... Args>
void ThreadPool::push_delay_task(int64_t delay_ms, Func &&f, Args&&... args)
{
    // expected maximum execute time
    auto exec_tm = Milliseconds() + delay_ms;
    std::unique_lock<std::mutex> lock(queue_mutex);
    if(sizeof...(Args))
    {
        delay_tasks.emplace(
            ThreadPool::TimeTask(exec_tm, std::bind(
                std::forward<Func>(f), std::forward<Args>(args)...
            ))
        );
    }
    else 
    {
        delay_tasks.emplace(ThreadPool::TimeTask(exec_tm, std::move(f)));
    }
    queue_condition.notify_one();
}

// return false if cant't add task
template<typename TaskContainer>
bool ThreadPool::push_batch_delay_task(int64_t delay_ms, TaskContainer &&task_list)
{
    if(task_list.empty())   return false;
    if(stop_flag.load())    return false;

    auto exec_tm = Milliseconds() + delay_ms;
    std::unique_lock<std::mutex> lock(queue_mutex);
    for(auto &task : task_list)
    {
        delay_tasks.emplace(ThreadPool::TimeTask(exec_tm, std::move(task)));
    }
    queue_condition.notify_all();
    return true;
}

// return the number of tasks in queue except of timed task
inline int ThreadPool::pending() const 
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    return tasks.size();
}

#endif 