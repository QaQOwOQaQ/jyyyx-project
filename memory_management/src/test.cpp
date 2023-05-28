#include <iostream>
#include <cstring>
#include <algorithm>
#include <stdlib.h>
#include <ext/pool_allocator.h>

using namespace std;

void moreMemory()
{
    cerr << "out of memory";

}

int main()
{
    set_new_handler(moreMemory);
    int *p = new int[100000000000000000];
    return 0;
}