#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(const char *s);

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("Usage : <port>\n");
        exit(1);
    }
    int serv_sock, clnt_sock;
    struct sockaddr_in clnt_addr, serv_addr;
    socklen_t clnt_len;
    char message[30];
    int read_cnt;

    // 1. create socket
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) 
        error_handling("socket() error!");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    // 2. bind and listen
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error!");
    if(listen(serv_sock, 5) == -1)
        error_handling("listen error!");
    // 3. accept
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_len);
    if(clnt_sock == -1)
        error_handling("accept() error!");

    clnt_len = sizeof(clnt_addr);
    for(int i = 0; i < 3; i ++ )
    {
        sleep(5);
        read_cnt = read(clnt_sock, message, 30);
        if(read_cnt == -1)
        {
            printf("read done!\n");
            break;
        }
        else 
        {
            printf("%dth read: %s\n", i + 1, message);
        }
    }
    // 4. close
    close(serv_sock);
    close(clnt_sock);
    return 0;
}

void error_handling(const char *s)
{
    fputs(s, stderr);
    fputc('\n', stderr);
    exit(1);
}