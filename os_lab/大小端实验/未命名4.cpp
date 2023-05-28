#include <stdio.h>
#include <iostream>

using namespace std;

int main()
{
	int a = 0x12345678;
	char b = a;
	
	printf("(int)a = 0x%x = %d\n", a, a);
	printf("(char)b = 0x%x = %d\n",b, b);
	
	int *c = &a;
	cout << &a << endl;
	cout << c << endl;

	if(120 == b)	printf("Ð¡¶Ë\n");
	else	printf("´ó¶Ë\n");	
	
	return 0;
}

