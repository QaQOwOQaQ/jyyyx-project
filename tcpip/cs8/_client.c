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
    FILE *fp;
    char buffer[BUF_SIZE];
    int read_cnt;
    struct sockaddr_in serv_addr;
    
    // 1. create socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    fp = fopen("copy.txt", "wba");
    if(sock == -1)
        error_handling("socket() error");
    
    // 2. bind    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // 3. connect
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    
    while((read_cnt = read(sock, buffer, BUF_SIZE)) != 0)
        fwrite((void*)buffer, 1, read_cnt, fp);
    fputs("Received file data!\n", stdout);
    write(sock, "Thank you!", 11);
    
    // 4. close
    fclose(fp);
    close(sock);
    return 0;
}

void error_handling(const char *s)
{
    fputs(s, stderr);
    fputc('\n', stderr);
    exit(1);
}