#include <iostream>
#include <myheader.h>

using namespace std;

#define V2
#define DEBUG

/* #error && defined()
* #error命令是C/C++语言的预处理命令之一
* 当预处理器预处理到#error命令时将停止编译并输出用户自定义的错误消息。
*/

#if defined(V1)
    #include <v1.h>
#elif defined(V2)
    #include <v2.h>
#elif defined(V3)
    #include <v3.h>
#else
    #error "[Error] no header included!";
#endif 

void test1()
{
    constexpr int TEST_COUNT = 10;
    Skiplist L;
    for(int i = 1; i <= TEST_COUNT; i ++ )
    {
        for(int j = 1; j <= i; j ++ )
            L.add(i);
    }
    for(int i = 1; i <= TEST_COUNT; i ++ )
    {
        if(L.search(i)) 
            cout << "i: " << i << " count = " << L.count(i) << endl;
    }
    for(int i = 1; i <= TEST_COUNT; i ++ )
    {
        for(int j = 1; j <= 5; j ++ )
            L.erase(i);
    }
    cout << "<=======after delete=======>\n";
    for(int i = 1; i <= TEST_COUNT; i ++ )
    {
        if(L.search(i)) 
            cout << "i: " << i << " count = " << L.count(i) << endl;
    }
}

int main()
{
    PRINT_BEGIN;
    test1();
    PRINT_END;
    return 0;
}