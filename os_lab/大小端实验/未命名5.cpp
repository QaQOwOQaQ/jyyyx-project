#include <iostream>

using namespace std;

int main()
{
	int a[5] = {1, 2, 3, 4, 5};
	
	cout << "����a����ʼ��ַ��" << &a << endl;
	cout << "����a��2��Ԫ�ص�ַ��" << &a + 1 << endl;
	cout << "����a��2��Ԫ�ص�ַ�е�Ԫ�ص�ֵ: " << *(a + 1) << endl;
	
}
