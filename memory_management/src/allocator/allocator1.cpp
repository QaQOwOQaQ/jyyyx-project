#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstring>
#include <stdlib.h>

#define TEST

/* 缺点
1. 移植性差：operator new/delete 的重载放在了 class 实现中，如果我们想在另一个类中添加分配器，就需要再写一次
2. 额外空间大：原本我们希望通过内存池设计，减少cookie的内存，但是我们却在class中添加了一个指针，原本只有一个 int 占 4 字节
              现在添加了一个指针变成了 16 字节，足足扩大了 4 倍，当然，在我们的 class 原本体积就很大时，例如 100 字节，
              那么 8 字节的指针大小其实是可以忽略的。但是我们应该有更好的办法解决 next 指针的问题。
*/

using namespace std;

class Foo {
public:
    Foo(int _val = 1024) : val(_val), next(nullptr) {}
    Foo(int _val, Foo *_next) : val(_val), next(_next) {}
    static void* operator new(size_t size);
    static void operator delete(void *ptr, size_t);
    static void grabage_collection();    // 清理空闲链表
public:
    static Foo* freeStore;               // 空闲链表
    static const int FooChunk = 24;      // 每次分配多少个Foo
private:
    static void allocate_new_memory(size_t size);
#ifdef TEST
public:
#endif 
    int val;
    Foo *next;  // 串连空闲链表
};

// 定义static成员时，不需要再加上static
Foo* Foo::freeStore = 0;
inline void Foo::allocate_new_memory(size_t size)
{
    if(freeStore)   return ;
#ifdef TEST
    cout << "Call allocate_new_memory!" << endl;
#endif 
    size_t chunk = FooChunk * size; 
    Foo *p = nullptr;
    p = freeStore = (Foo*)malloc(chunk);
#ifdef TEST
    int count = 0;
#endif
    for(p = freeStore; p != &freeStore[FooChunk - 1]; ++ p)
    {
#ifdef TEST
        cout << "[" << ++ count << "]" << " p: " << p << "; p + 1: " << p + 1 << endl;
#endif 
        p->next = p + 1;
    }
    p->next = 0;
}

inline void Foo::grabage_collection() // cant run！
{
    if(freeStore == nullptr)    return ;
    cout << "can free" << endl;
    void *ptr = (void*)freeStore;
    free(ptr);
}

inline void* Foo::operator new(size_t size)
{
    Foo *p = nullptr;
    if(freeStore == nullptr)
        allocate_new_memory(size);
    p = freeStore;
    freeStore = freeStore->next;
    return p;
}

void Foo::operator delete(void *p, size_t)
{
    static_cast<Foo*>(p)->next = freeStore;
    freeStore = static_cast<Foo*>(p);
}

void test1()
{
    cout << "size: " << sizeof(Foo) << endl;
    static int n = 1024;
    Foo *p[n];
    for(int i = 0; i < n; i ++ )
    {
        p[i] = new Foo(i + 1);
        cout << "[" << i << "] now addr: " << p[i] << endl;
    }
    for(int i = 0; i < n; i ++ )
    {
        delete p[i];
    }

    Foo *tra = Foo::freeStore;
    int count = 0;
    while(tra != nullptr)
    {
        cout << "[" << ++count << "] " << tra->val << endl;
        tra = tra->next;
    }
}

void test2()
{
    Foo *p[10];
    for(int i = 0; i < 10; i ++ )
    {
        p[i] = ::new Foo();
        cout << p[i] << endl;  
    }
}

int main()
{
    test1();
    puts("Test end ...");
    return 0;
}