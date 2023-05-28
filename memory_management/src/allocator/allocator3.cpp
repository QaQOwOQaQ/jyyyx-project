#include <iostream>
#include <algorithm>
#include <cstring>
#include <stdlib.h>
#include <assert.h>

#define TEST

using namespace std;

class MyAllocator {
public:
    void* allocate(size_t size);
    void deallocate(void *ptr, size_t);
private:
    struct obj {
        struct obj *next; // embeded pointer(嵌入式指针)
    };
#ifdef TEST
public:
#endif 
    obj* freeStore = nullptr;
    const size_t CHUNK = 5;
};

class Foo {
public:
    // Foo() = default;
    Foo(int _cnt = 1024, double _price = 10.25) 
    : count(_cnt), price(_price) {}
    ~Foo() { cout << "[Dtor] " << this << endl; }
    int getCount() const { return count; }
    double getPrice() const { return price; }
public:
    static MyAllocator myAlloc;
    static void* operator new(size_t size)
    {   
        if(size != sizeof(Foo))
        {
            cout << "alloc error" << endl;
            return ::operator new(size);
        }
        return myAlloc.allocate(size);  
    }
    static void operator delete(void *ptr, size_t size)
    {
        if(size != sizeof(Foo))
        {
            cout << "delete error" << endl;
            return ::operator delete(ptr, size);   
        }
        return myAlloc.deallocate(ptr, size); 
    }
private:
#ifdef TEST
public:
#endif
    int count;
    double price;
};

MyAllocator Foo::myAlloc;

void* MyAllocator::allocate(size_t size)
{
    obj *p = nullptr;
    if(freeStore == nullptr)
    {
        cout << "call new memory!" << endl;
        size_t chunk = CHUNK * size;
        void *mem = malloc(chunk);
        assert(mem);
        p = freeStore = static_cast<obj*>(mem);
        for(size_t i = 0; i < CHUNK - 1; i ++ )
        {
            // [WARNNING] [BUG]
            // 注意不可能写为下面的形式
            // p[i].next = &p[i + 1];
            // 因为p虽然是一个obj对象，但是我们要把它当做一个Foo对象
            // 所以我们要移动的距离是sizeof(Foo)
            // 但是在上面的形式中，我们移动的距离是sizeof(obj)
            p->next = (obj*)((char*)p + size);
            p = p->next;
        }
        p->next = nullptr;
    }
    p = freeStore;
    freeStore = freeStore->next;
    return p;
}
void MyAllocator::deallocate(void *ptr, size_t)
{
    static_cast<obj*>(ptr)->next = freeStore;
    freeStore = static_cast<obj*>(ptr);
}

void test()
{
    cout << "SIZE: " << sizeof(Foo) << endl;
    static int N = 16;
    Foo *f[N];
    for(int i = 0; i < N; i ++ )
    {
        // f[i] = new Foo(Foo(i, i * 1.1)); 
        f[i] = new Foo;
        cout << "Ctor[" << i << "] " << f[i] << endl;
    }
    for(int i = 0; i < N; i ++ )
    {
        f[i]->count = (i + 1);
        f[i]->price = (i + 1) * 1.1;
    }
    for(int i = 0; i < N; i ++ )
    {
        cout << f[i]->getCount() << ' ' << f[i]->getPrice() << endl; 
    }
    for(int i = 0; i < N; i ++ )
        delete f[i];
    auto p = Foo::myAlloc.freeStore;
    int counter = 0;
    while(p && counter < N)
    {
        cout << "cur: " << p << endl;
        p = p->next;
        counter ++ ;
    }
}

void test2()
{
    for(int i = 0; i < 10; i ++ )
    {
        // Foo *f = new Foo(i, i + 10);
        // cout << "addr: " << f << ' ' << f->getCount() << ' ' << f->getPrice() << endl;
        // delete f;
    }
    // Foo *f = new Foo(1, 1024);
    // cout << "addr: " << f << ' ' << f->getCount() << ' ' << f->getPrice() << endl;
}

int main()
{
    test();
    test2();
    puts("Test end......");
}