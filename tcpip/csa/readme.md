# 套接字的多种选项

先断开连接的一段有 TIME_WAIT。
因此如果服务器先断开连接的话，有一小段时间再次连接会错误：bind error

之所以有这个 TIME-WAIT，主要是因为此时客户端发送 FIN 时，服务器最后发送的 ACK 可能会丢失。

如果丢失的话，客户端就会发送重复 FIN，此时服务器的 TIME-WAIT 会重启。

因此如果 ACK 丢失的比较多的话（网络状况不理想），TIME-WAIt 可能比较长。

如果我们不想等待这段时间，可以修改套接字的 SO_REUSEADDR 的状态。
``` c++
option = TRUE;
optlen = sizeof(option);
setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, 
    (void*)&option, optlen);
```