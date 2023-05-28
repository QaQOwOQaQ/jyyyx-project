# TCP 回声服务器

## 头文件<unistd>
```c++
write and read defined in <unistd.h>
```

## 会送地址
127.0.0.1 表示回送地址，指的是计算机自身的IP地址，无论什么程序，一旦使用回送地址发送数据，协议软件**立即返回,不进行任何网络传输.**

## BUG
``` c++
while(1)
{
    fgets(message, SIZE, stdin);
    if(!strcmp("q", message) || !strcmp("Q", message))
    {
        break;   
    }
}

```        
上面的代码看似没什么问题，当我们输入 q 或者 Q 时程序将结束，但是我们忽略了 fgets 的一个特性：**它会读入回车**。
注意 gets 不会读入回车。
这将导致程序死循环，因为我们输入的 message 长度恒大于1，因为至少存在一个回车。
我们可以在 while 中加入测试语句打印 message 的长度，当我们输入 q 的时候，输出长度等于 2，也就是 "q\n" 的长度！
所以说 if 要改为以下：
``` c++
if(!strcmp("q\n", message) || !strcmp("Q\n", message))
```