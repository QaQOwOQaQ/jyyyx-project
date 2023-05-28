#include <iostream>
#include <algorithm>
#include <cstring>
#include <stdlib.h>
#include <assert.h>

#define TEST

using namespace std;

class Foo {
public:
    static void *operator new(size_t size);
    static void operator delete(void *ptr, size_t);
    static Foo* freeStore;
    static const size_t chunkSize = 24;

public:
    Foo(int _val = 0) : val(_val) {}
    ~Foo();
    int get() const { return val; }
private:
    static void allocateNewMemory(size_t size);
#ifdef TEST
public:
    // 节省内存
    union {
        int val;
        Foo *next;
    };
#endif 
};

Foo *Foo::freeStore = nullptr;

inline void Foo::allocateNewMemory(size_t size)
{
    size_t chunk = chunkSize * size;
    // 不要直接在函数中 new/malloc
    void *mem = malloc(chunk);
    assert(mem);
    Foo *p = nullptr;
    p = freeStore = static_cast<Foo*>(mem);
    for(size_t i = 0; i < chunkSize - 1; i ++ )
        p[i].next = p + (i + 1);
    p[chunkSize - 1].next = nullptr;
}

inline void *Foo::operator new(size_t size)
{
    // 内存分配有误，交给全局 operator new 处理
    if(size != sizeof(Foo))
    {
        cout << "size[" << size << "] not match" << endl; 
        return ::operator new(size);
    }
    Foo *p = nullptr;
    if(freeStore == nullptr)
        allocateNewMemory(size);
    assert(freeStore);
    p = freeStore;
    freeStore = freeStore->next;
    return p;
}
inline void Foo::operator delete(void *ptr, size_t size)
{
    // 可以删除nullptr
    if(ptr == nullptr)  return ;
    if(size != sizeof(Foo))
        return ::operator delete(ptr);
    // 删除只是重新添加到空闲链表
    static_cast<Foo*>(ptr)->next = freeStore;
    freeStore = static_cast<Foo*>(ptr);
}

Foo::~Foo()
{
    static int dtorCount = 0;
    cout << "[" << dtorCount << "] " << "dtor: " << val << endl;
}

void test()
{
    static int n = 32;
    Foo* f[n];
 
    for(int i = 0; i < n; i ++ )
    {
        f[i] = new Foo(i + 10);
        // 间隔只有8，而不是16了！
        // 另外，不同的内存大块之间是不连续的
        // 我们每次只分配24个内存小块
        // 所以说每24个小块之间可能不连续
        cout << "Ctor[" << i << "] " << f[i] << ", val= " << f[i]->get() <<  endl;
    }
    for(int i = 0; i < n; i ++ )
        delete(f[i]);
    int freeCount = 0;
    Foo *p = Foo::freeStore;
    while(p)
    {
        cout << "Del[" << ++freeCount << "] " << p << ", val= " << p->get() << endl;
        p = p->next;
        if(freeCount >= 32)  
        {
            break ;
        }
    }
}

int main()
{
    puts("Test end....");
    test();
    return 0;
}