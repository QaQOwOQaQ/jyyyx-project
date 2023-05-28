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
    if(argc != 3)
    {
        printf("Usage : <IP> <Port>\n");
        exit(1);
    }
    int sock;
    struct sockaddr_in serv_addr;
    char message[30] = "Hello,Server,goodnight!";
    int read_cnt, str_len, read_len;

    // 1. create sock
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error!");
    
    // 2. connect
    memset(&serv_addr, 0, sizeof serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");

    while(1)
    {
        fputs("pls input your message(Q/q to quit): ", stdout);
        fgets(message, SIZE, stdin);
        if(strcmp("q\n", message) == 0 || strcmp("Q\n", message) == 0)
        {
            break;   
        }
        str_len = write(sock, message, strlen(message));
        read_len = 0;
        while(read_len < str_len)
        {
            read_cnt = read(sock, &message[read_len], SIZE - 1);
            if(read_cnt == -1)
                error_handling("read() error!");
            read_len += read_cnt;
        }
        message[read_len] = '\0';
        printf("Message from server: %s", message);
    }
    // 3. close
    close(sock);
}

void error_handling(const char *s)
{
    fputs(s, stderr);
    fputc('\n', stderr);
    exit(1);
}
