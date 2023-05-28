#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE (1024) 
#define NUM_SIZE (4)
#define NUM_COUNT (8)

void error_handling(const char *s);
int calculate(char s[]);

int main(int argc, char ** argv)
{   
    if(argc != 2) 
    {
        printf("Usage : <Port>\n");
        exit(1);
    }
    // 0. pre declaration
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_len;
    char message[BUF_SIZE];
    int recv_cnt;


    // 1. create socket
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    
    // 2. bind server
    memset(&serv_addr, 0, sizeof serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    // 3. listen
    if(listen(serv_sock, 2) == -1)
        error_handling("listen() error");

    // 4. accpet
    clnt_len = sizeof(clnt_addr);
    for(int i = 0; i < 5; i ++ )
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_len);
        if(clnt_sock == -1)
            error_handling("accept() error");
        recv_cnt = read(clnt_sock, message, BUF_SIZE);
        if(recv_cnt == -1) 
            error_handling("read() error");
        
        int res = calculate(message);   
        write(clnt_sock, (char *)&res, sizeof(res));
    }
    
    // 5. close
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

int calculate(char message[])
{
    printf("%s", message);
    int operand_cnt = 0;
    int operand[NUM_COUNT];
    char operator;
    int res = 0;

    operand_cnt = message[0];
    
    if(operand_cnt == 0)    return 0;

    for(int i = 0; i < operand_cnt; i ++ )
        operand[i] = message[NUM_SIZE + NUM_SIZE * i];
    
    operator = message[NUM_SIZE + NUM_SIZE * operand_cnt + 1];
    
    switch(operator)
    {   
        case '+':
            for(int i = 0; i < operand_cnt; i ++ ) 
                res += operand[i];
            break;
        case '-':
            res = operand[0];
            for(int i = 1; i < operand_cnt; i ++ )
                res -= operand[i];
            break;
        case '*':
            res = 1;
            for(int i = 0; i < operand_cnt; i ++ )
                res *= operand[i];
            break;
        default:
            printf("Error operator\n");
    }
    printf("res = %d\n", res);
    return res;
}