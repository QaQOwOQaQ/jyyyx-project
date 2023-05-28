// 多进程并发服务器
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>

#define BUF_SIZE (1024)

void error_handling(const char *s);
void read_childproc(int sig);

int main(int argc, char **argv)
{
    // 0. pre declaration
    if(argc != 2)
    {
        printf("Usage : <Port>\n");
        exit(1);
    }
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_len;
    pid_t pid;
    struct sigaction act;
    char message[BUF_SIZE];
    int state, str_len;

    // 1. create socket and register signal_handler
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);
    if(state == -1)
        error_handling("sigaction() error");
    // 2. bind
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    // 3. listen
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    
    // 4. accept
   
    // 5. echo
    while(1)
    {
        clnt_len = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_len);
        if(clnt_sock == -1)
            continue;
        else
            puts("new client connected...");
        // fork a new process for new client
        pid = fork();
        if(pid == -1)
        {
            close(clnt_sock);
            continue;
        }
        if(pid == 0) // child process, provide echo service
        {
            close(serv_sock);
            while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
                write(clnt_sock, message, str_len);
            close(clnt_sock);
            puts("client disconnetced...");
            return 0; // end child process and return a ret_value
        }
        else    
            close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

void read_childproc(int sig)
{   
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("remove proc id: %d\n", pid);
}

void error_handling(const char *s)
{
    fputs(s, stderr);
    fputc('\n', stderr);
    exit(1);
}
