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
    if(argc != 2)
    {
        printf("Usage <Port>\n");
        exit(1);
    }
    int serv_sock, clnt_sock;
    FILE *fp;
    char buffer[BUF_SIZE];
    int read_cnt;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_len;
    // 1. create socket and open file
    fp = fopen("data.txt", "rb");
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
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
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    
    // 4. accept
    clnt_len = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_len);
    if(clnt_sock == -1)
        error_handling("accept() error");

    // 5. data process
    while(1)
    {
        read_cnt = fread((void*)buffer, 1, BUF_SIZE, fp);
        if(read_cnt < BUF_SIZE)
        {
            write(clnt_sock, buffer, read_cnt);
            break;
        }
        write(clnt_sock, buffer, BUF_SIZE);
    }

    // 6. close
    shutdown(clnt_sock, SHUT_WR);
    read(clnt_sock, buffer, BUF_SIZE);
    printf("Message from client: %s\n", buffer);

    fclose(fp);
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(const char *s)
{
    fputs(s, stderr);
    fputc('\n', stderr);
    exit(1);
}