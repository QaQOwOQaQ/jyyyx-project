#include <stdio.h>
//�����Ӻ���һЩC����ָ����÷������͸�ˣ�Ӧ�ÿ���ѧ�㶫����
int main(void)
{
 int a = 1;
 char *p = NULL;
 p = (char *)&a;
 printf("*p = %d\n",*p);
 if(*p == 1)
 {
  printf("��ǰ��С��ģʽ\n");
 }
 else
 {
  printf("��ǰ�Ǵ��ģʽ\n");
 }
 return 0;
}

