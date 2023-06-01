#include <iostream>
#include <cassert>

using namespace std;

void test1()
{
    int a = 0x1234;
	char *b = (char *)&a;
	printf("0x%lx = %x\n", (unsigned long)&a, b[0]);
	printf("0x%lx = %x\n", (unsigned long)&a + 1, b[1]);
}

void test2()
{
    union 
    {
        short s;
        char c[sizeof(short)];
    } un;
    un.s = 0x0102;
    assert(sizeof(short) == 2);
    if(un.c[0] == 2 && un.c[1] == 1)
        cout << "small endian!\n";
    else 
        cout << "big endian\n";
}

int main()
{
    // small endian: small address store lower bit
    // big endian: small address store higher bit
    test2();   
}