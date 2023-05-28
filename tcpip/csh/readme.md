# 十四章、广播和多播
在多播和广播中，客户端与服务器端被称为接收方和发送方。

## 1. 多播 multicast
服务端需要设置 TTL
接受端需要加入 多播组

设置 TTL：通过 setsockopt
``` c++
int send_sock;
int time_live = 64;
// ...
send_sock = socket(PF_INET, SOCK_DGRAM, 0);
// ...
setsockopt(send_sock, IPPORT_IP, IP_MULTICAST_TTL, 
    (void*)&time, sizeof(time_live));
// ...
```

加入多播组：通过 setsockopt 和 struct ip_mreq
``` c++
int recv_sock;
struct ip_mreq join_addr;
// ...
revc_sock = socket(PF_INET, SOCK_DGRAM, 0);
// ...
join_addr.imr_multiaddr.s_addr = "多播组地址信息";
join_addr.imr_interface.s_addr = "加入多播组的主机地址信息";
setsockopt(rev_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
    (void*)&join_addr, sizeof(join_addr);
// ...
```
struct ip_mreq:
``` C++
struct ip_mreq
{
    struct in_addr imr_multiaddr; //写入加入组的IP地址
    struct in_addr imr_interface; //加入该组的套接字所属主机的IP地址
};
```
