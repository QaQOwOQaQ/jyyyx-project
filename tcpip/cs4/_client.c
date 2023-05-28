#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE  (1024)
#define NUM_SIZE  (4)
#define NUM_COUNT (8)

void error_handling(const char *s);

int main(int argc, char **argv)
{
    if(argc != 3) 
    {
        printf("Usage : <IP> <Port>\n");
        exit(1);
    }
    // 0. pre declare 
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];

    // 1. create socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");
    
    // 2. bind
        // dont nedd 

    // 3. connect
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");

    // 4. transmit 
        // 4.1 read datas
    printf("pls input the count of numbers: ");
    scanf("%d", (int *)&message[0]); 
    printf("read_cnt = %d\n", message[0]);
    if(message[0] > NUM_COUNT)
        error_handling("you want read too much numbers!");
    for(int i = 0; i < message[0]; i ++ ) 
    {
        printf("pls input %dth operand: ", i + 1);
        scanf("%d", (int *)&message[NUM_SIZE + i * NUM_SIZE]);
    }
    for(int i = 0; i < message[0]; i ++ )
    {
        printf("%dth = %d\n", i + 1, message[4 + 4 * i]);
    }
    fgetc(stdin);   // 接受一个会车！！
    printf("pls input operator: ");
    scanf("%c", &message[message[0] * NUM_SIZE + NUM_SIZE + 1]);

        // 4.2  write to server
    write(sock, message, BUF_SIZE);
    int result;
    read(sock, &result, NUM_SIZE);
    printf("cal res = %d\n", result);
    fputc('\n', stdout); 
    
    // 5. close 
    close(sock);
    return 0;
}

void error_handling(const char *s)
{
    fputs(s, stderr);
    fputc('\n', stderr);
    exit(1);
}