#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

#define SIZE (1024)

void error_handling(const char *s);

int main(int argc, char **argv)
{
    // 0. pre declaration
    if(argc != 2)
    {
        printf("Usage: <Port>\n");
        exit(1);
    }
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_len;

    char buffer[SIZE];
    int str_len;
    
    fd_set reads, cpy_reads;
    int fd_max, fd_num;
    struct timeval timeout;
    
    // 1. init socket and address
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    // 2. bind
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    // 3. listen
    if(listen(serv_sock, 3) == -1)
        error_handling("listen() error");

    // 4. set fd
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock; // last created fd

    // 5. accept
    while(1)
    {
        cpy_reads = reads;
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;

        // if any read(client connect) event happend
        fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout);
        if(fd_num == -1)
            error_handling("select() error");
        if(fd_num == 0)
        {
            printf("Time-out\n");
            continue;
        }
        for(int i = 0; i < fd_max + 1; i ++ )
        {
            // if fd is serv_sock, accept
            if(FD_ISSET(i, &cpy_reads))
            {       
                if(i == serv_sock)
                {
                    clnt_len = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_len);
                    if(clnt_sock == -1)
                        error_handling("accept() error"); 
                    FD_SET(clnt_sock, &reads);
                    if(clnt_sock > fd_max)   
                        fd_max = clnt_sock;
                    printf("Connected client: %d\n", clnt_sock);
                }
                else    // not serv_sock, so is clnt_sock, read data
                {
                    str_len = read(i, buffer, SIZE);
                    if(str_len == 0)
                    {
                        // client write done
                        FD_CLR(i, &reads);
                        close(i);
                        printf("Closed client: %d\n", i);
                    }
                    else 
                        write(clnt_sock, buffer,str_len);
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}

void error_handling(const char *s)
{
    fputs(s, stderr);
    fputc('\n', stderr);
    exit(1);
}