#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SIZE (32)

int main(int argc, char **argv)
{
    int recv_sock;
    int str_len;
    char buf[SIZE];
    struct sockaddr_in addr;
    struct ip_mreq join_addr;
    if(argc != 3)
    {        
        printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
        exit(1);
    }
    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);   // local ip addr
    addr.sin_port = htons(atoi(argv[2]));

    bind(recv_sock, (struct sockaddr*)&addr, sizeof(addr));

    join_addr.imr_multiaddr.s_addr = inet_addr(argv[1]);   // multicast group addr
    join_addr.imr_interface.s_addr = htonl(INADDR_ANY);    // local addr
    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
            (void*)&join_addr, sizeof(join_addr));

    while(1)
    {
        // because we bind addr
        // so we can set paramete5 and 6 as null and 0
        str_len = recvfrom(recv_sock, buf, SIZE, 0, NULL, 0);   
        if(str_len <= 0)
            break;
        buf[str_len] = 0;
        fputs(buf, stdout);
    }
    close(recv_sock);
    return 0;
}