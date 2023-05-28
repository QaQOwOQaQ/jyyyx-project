#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SIZE (30)

void error_handling(const char *s);

int main(int argc, char **argv) 
{
    if(argc != 2)
    {
        printf("Usage : <Port>\n");
        exit(1);
    }

    int clnt_sock, serv_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_len;
    char message[SIZE];
    int read_cnt;
    // 1. create socket
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error!");
    // 2. bind and listen
    memset(&serv_addr, 0, sizeof serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error!");
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error()");
    // 3. accept
    clnt_len = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_len);
    if(clnt_sock == -1)
        error_handling("accept() error!");
    while(1)
    {
        read_cnt = read(clnt_sock, message, sizeof(message));
        if(read_cnt == 0)
            break;
        write(clnt_sock, message, read_cnt);
    }
    // 4. close
    close(clnt_sock);
    close(serv_sock);
}

void error_handling(const char *s)
{
    fputs(s, stderr);
    fputc('\n', stderr);
    exit(1);
}