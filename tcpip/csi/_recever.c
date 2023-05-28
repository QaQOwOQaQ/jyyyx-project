#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE (32)

/* .不写错误处理函数了 */

int main(int argc, char **argv)
{
    int recv_sock;
    int str_len;
    char buffer[BUF_SIZE];
    struct sockaddr_in addr;
    if(argc != 2)
    {
        printf("Usage : <Port>\n");
        exit(1);
    }

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));
    
    // bind 到广播
    bind(recv_sock, (struct sockaddr*)&addr, sizeof(addr));

    while(1)
    {
        str_len = recvfrom(recv_sock, buffer, BUF_SIZE - 1, 0, NULL, 0);
        if(str_len < 0) break;
        buffer[str_len] = 0;
        fputs(buffer, stdout);
    }
    close(recv_sock);
    return 0;
}