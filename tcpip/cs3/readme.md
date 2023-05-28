## 第五章 LAb
实现一个在服务器端执行算术运算的服务器
客户端流程：
1. 输入计算的数字的个数
2. 依次输入每一个数字
3. 输入执行的运算（+，-，*）

服务器：
接收 operand 和 operator
输出结果。

## 神仙语法
``` c++
#include <stdio.h>
#include <string.h>

int main()
{
    char message[100] = "\0";
    for(int i = 0; i < 4; i ++ ) {
        scanf("%d", (int*)&message[i*4]);
    }
    for(int i = 0; i < 4; i ++ ) {
        int a = message[i*4];
        printf("%d ", a);
    }
    return 0;
}
/*
input: 1 2 3 4
output: 1 2 3 4 
*/
```