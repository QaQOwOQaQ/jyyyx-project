# 第十二章、I/O 复用服务器

#  一、复用
为了提高物理设备的效率，用最少的物理要素传递最多的数据时采用的技术。

# 二、select函数
select 函数是最具代表性的实现实用多路复用服务器的方法。

## 1. select 函数的监视事件
1. 是否存在套接字**接受**数据？
2. 无需阻塞**传输**数据的套接字有哪些？
3. 那些套接字发生了**异常**

## 2. 调用过程
步骤一：
1. 设置文件描述符
2. 指定监视范围
3. 设置超时
步骤二：
调用 select 函数
步骤三：查看调用结果

### 设置文件描述符
fd_set：一个结构体的typedef，内部是一个数组。
宏：
1. FD_ZERO  (fd_set *fdset);
2. FD_SET   (int fd, fd_set *fdset);
3. FD_CLR   (int fd, fd_set *fdset);
4. FD_ISSET (int fd, fd_set *fdset);

``` C++
int main()
{
    fd_set set;
    FD_ZERO(&set);
    FD_SET(1, &set);
    FD_SET(2, &set);
    FD_CLR(2, &set);
}
```

### 设置监视范围和超时
select 函数的定义：
``` c++
#include <sys/select.h>
#include <sys/time.h>
int select(int maxfd, fd_set *readset, 
            fd_set *writeset, fd_set *exceptset, 
            const struct timeval *timeout);
// 返回值：发生错误返回 -1，超时返回0，否则返回发生事件的文件描述符的数量。
// maxfd：监视对象文件描述符数量，一般为最大的文件描述符的编号+1，因为文件描述符的编号从0开始
// readset，writeset，exceptset 分别代表三种监听事件
// timeout：为了防止调用 select 函数后陷入无限阻塞的状态，传递超时（time-out）信息。
```

### struct timeval
``` c++
struct timeval
{
    long tv_sec;
    long tv_usec;
}

```

## 3. 注意
select 函数每次执行都会修改监听事件的 struct fd_set 和超时的 struct timeval。
因此每次执行 select 函数时都需要重新设置 struct fd_set 和 struct timeval。


## 4. 测试代码
`select.c`



# 三、基于 select 实现 I/O 复用的复用服务器
