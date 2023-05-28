#include <stdio.h>
//本例子含有一些C语言指针的用法，理解透了，应该可以学点东西。
int main(void)
{
 int a = 1;
 char *p = NULL;
 p = (char *)&a;
 printf("*p = %d\n",*p);
 if(*p == 1)
 {
  printf("当前是小端模式\n");
 }
 else
 {
  printf("当前是大端模式\n");
 }
 return 0;
}

