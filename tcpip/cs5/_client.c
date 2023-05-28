#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE (30)

void error_handling(const char *s);

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        printf("Usage : <IP> <Port>\n");
        exit(1);
    }
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t clnt_len;
    struct sockaddr_in serv_addr, clnt_addr;
    
    // 1. create UDP socket
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error_handling("socket() error");
    
    // 2. (bind) server
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // 3. write and read
    while(1)
    {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        sendto(sock, message, sizeof(message), 0,
                (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        clnt_len = sizeof(clnt_addr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, 
                    (struct sockaddr*)&clnt_addr, &clnt_len);
        message[str_len] = 0;
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