#include <iostream>

using namespace std;

int main()
{
	int a = 0x1234;
	char *b = (char *)&a;
	printf("0x%lx = %x\n", (unsigned long)&a, b[0]);
	printf("0x%lx = %x\n", (unsigned long)&a + 1, b[1]);
	
	return 0;
}
