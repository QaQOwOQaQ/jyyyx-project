#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE (1024)

void error_handling(const char *s);

int main(int argc, char **argv)
{
    // 0. pre declaration
    int sock;
    struct sockaddr_in serv_addr;
    int str_len;
    char message[BUF_SIZE];
    if(argc != 3)
    {
        printf("Usage : <IP> <Port>\n");
        exit(1);
    }
    
    // 1. create socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error!");
    
    // 2. connect server
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");
    else 
        puts("Connected....\n");
    
    // 3. echo
    while(1)
    {
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);
        if(!strcmp("q\n", message) || !strcmp("Q\n", message))
            break;

        write(sock, message, strlen(message));
        str_len = read(sock, message, BUF_SIZE - 1);
        if(str_len == -1)   
            error_handling("read() error");
        message[BUF_SIZE - 1] = 0;
        printf("Message from server: %s", message);
    }

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