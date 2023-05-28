#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(const char *s);

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("Usage : %s <IP> <port>", argv[0]);
        exit(1);
    }

    int sock;
    struct sockaddr_in serv_addr;
    char message1[30] = "I am client!";
    char message2[30] = "Hello, server!";
    char message3[30] = "Oh my god!";

    // 1. create socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket error()\n");
    // 2. connnetc server
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_family = AF_INET;
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    // 3. read and write
    write(sock, message1, 30);
    write(sock, message2, 30);
    write(sock, message3, 30);
    
    // 4. close
    close(sock);
    return 0;
}

void error_handling(const char *s) 
{
    fputs(s, stderr);
    fputc('\n', stderr);
    exit(1);
}