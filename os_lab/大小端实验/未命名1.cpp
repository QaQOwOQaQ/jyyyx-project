#include <iostream>

using namespace std;

int main()
{
	int x = 1;
	char *p = (char *)&x;
    printf("*p = %d\n",*p);
    
	if(*p)	printf("С��\n");
	else	printf("���\n");

	return 0;
}
