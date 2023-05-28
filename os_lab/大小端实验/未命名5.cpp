#include <iostream>

using namespace std;

int main()
{
	int a[5] = {1, 2, 3, 4, 5};
	
	cout << "数组a的起始地址：" << &a << endl;
	cout << "数组a第2个元素地址：" << &a + 1 << endl;
	cout << "数组a第2个元素地址中的元素的值: " << *(a + 1) << endl;
	
}
