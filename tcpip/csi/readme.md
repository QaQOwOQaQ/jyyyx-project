# 广播服务器
和多播一样，广播也基于 UDP。

注意广播不是只能想本网络中的所有主机发送数据。它是可以想**某个**网络中的所有主机发送数据。因此它不能跨网络。

根据传输数据时使用的 IP 地址形式，广播分为：
1. 本地广播 local broadcast。IP：255.255.255.255。
2. 直接广播 diredted broadcase。执行网络位，主机位全1。

可以发现，广播和 udp 几乎一毛一样，只不过 IP 地址的格式比较特殊。因此我们需要通过设置套接字选项让 OS 知道这是广播而不是普通的 UDP 单播。
``` c++
int send_sock;
int bcast;
//...
send_sock = socket(PF_INET, SOCK_DGRAM, 0);
//...
setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, 
    (void*)&bcast, sizeof(bcast));
//...
```


# bug1

# bug2
真傻逼，htons 写成 htonl，debug 10分钟。

