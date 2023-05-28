#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE (30)

void error_handling(const char *s);

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("Usage : <Port>\n");
        exit(1);
    }
    int serv_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_len;
    char message[BUF_SIZE];
    int str_len;

    // 1. create socket
    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

    // 2. bind
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
        
    // 3. listen
        // 因为不是 tcp，发送方没有 connect
        // 接收方也就不需要监听

    // 4. accept
    while(1)
    {
        clnt_len = sizeof(clnt_addr);
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0
                    , (struct sockaddr*)&clnt_addr, &clnt_len);
        sendto(serv_sock, message, str_len, 0,
                (struct sockaddr*)&clnt_addr, clnt_len);
    }
    
    // 5. close
    close(serv_sock);

    return 0;
}

void error_handling(const char *s)
{
    fputs(s, stderr);
    fputc('\n', stderr);
    exit(1);
}