#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>


int _getsocjopt(int sock, int level, int optname, 
        void *optval, socklen_t *optlen);
int _setsockopt(int sock, int level, int optname,
        void *optval, socklen_t *optlen);


int main()
{
    int tcp_sock, udp_sock;
    int sock_type;
    socklen_t optlen;
    int state;

    optlen = sizeof(sock_type);
    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
    printf("SOCK_STREAM: %d\n", SOCK_STREAM);
    printf("STREAM_DGRAM: %d\n", SOCK_DGRAM);

    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, 
            (void *)&sock_type, &optlen);
    printf("Socket type one: %d\n", sock_type);
    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, 
            (void *)&sock_type, &optlen);
    printf("Socket type one: %d\n", sock_type);
    return 0;
}