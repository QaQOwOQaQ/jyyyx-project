#include <myheader.h>

using namespace std;

// 必须使用 ATOMIC_FLAG_INIT 初始化，否则值是 UB
// 即，既不是 set 也不是 clear
// 该宏将使得 atomic_flag 处于 clear 状态
std::atomic_flag alock = ATOMIC_FLAG_INIT;

void f(int n)
{
    for(int i = 0; i < 3; i ++ )
    {
        while(alock.test_and_set(std::memory_order_acquire)) // 获得锁
        ;   // spin
        std::cout << "Output from thread " << n << "\n";
        alock.clear(std::memory_order_release);  // 释放锁
    }
}

int main()
{   
    PRINT_BEGIN;
    std::vector<std::thread> v;
    for(int i = 0; i < 10; i ++ )
        v.emplace_back(f, i); // 通过 emplace 传入线程和参数
    for(auto &it : v)   
        it.join();
    PRINT_END;  
    return 0;
}