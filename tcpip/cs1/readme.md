# 最简单的 TCP CS
功能：客户端连接，服务器应答一下。

# BUG
bug代码,该代码会导致客户端 connect 超时：
``` c
serv_addr.sin_addr.s_addr = htonl(inet_addr(argv[1]));
```
改正后的代码：
``` c
serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
```
可以发现两个代码的不同之处在于，出 bug 的代码对 inet_addr() 转换后的代码做了一次网络字节序转换，按理说应该没问题啊。

我们做个小测试：
``` c
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    char *s = "1.2.3.4";    // 0x01020304：小端
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(s);
    printf("%x\n", addr.sin_addr.s_addr);   // 0x04030201:大端
    printf("%s\n", inet_ntoa(addr.sin_addr));   // 1.2.3.4
    addr.sin_addr.s_addr = htonl(inet_addr(s));
    printf("%x\n", addr.sin_addr.s_addr);   // 0x01020304：小端
    printf("%s\n", inet_ntoa(addr.sin_addr));   // 1.2.3.4
    
}
```
在测试结果中可以发现，直接使用 inet_addr 转换后的结果是大端（也就是网络字节序），并且再转换为字符串也是原来的地址
但是使用 htonl 之后，发现字节序没有变！也就是说 htonl 把 inet_addr 的转换结果又转换了一次
大概是因为 htonl 把 inet_addr(s) 这个整体看做一个小端，然后又做了一次字节序转换，整个流程就是：小端->大段->小端
因此说这里得到的地址不对，自然 connect 超时。
附：inet_addr 得到的字节序已经是网络字节序，不要画蛇添足加上 htonl！
``` c
/*Convert Internet host address from numbers-and-dots notation in CP
   into binary data in network byte order.  */
extern in_addr_t inet_addr (const char *__cp) __THROW;
```