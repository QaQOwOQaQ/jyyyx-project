#include <iostream>

using namespace std;

int main()
{
	int x = 1;
	char *p = (char *)&x;
    printf("*p = %d\n",*p);
    
	if(*p)	printf("Ð¡¶Ë\n");
	else	printf("´ó¶Ë\n");

	return 0;
}
