#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#define SIZE (32)

void read_childproc(int sig)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("remove proc id: %d\n", pid);
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        printf("Usage : <IP>\n");
        exit(1);
    }
    // 0. pre declaration
        // socket
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_len;
        // IPC
    int fds[2];
    pid_t pid;
        // signal_handler
    struct sigaction act;
        // date 
    int str_len;
    char buf[SIZE];

    // 1. init
        // socket
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
        // register sigaction
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);
        // bind IP and Port
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    // 2. bind
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // 3. listen
    listen(serv_sock, 5);

    // 4. create pipe
    pipe(fds);
    pid = fork();
    if(pid == 0)
    {
        FILE *fp = fopen("mes.txt", "wt");
        char message[SIZE];
        for(int i = 0; i < 10; i ++ )
        {
            int len = read(fds[0], message, SIZE);
            fwrite((void*)message, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }
    else 
    {
        clnt_len = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_len);
        pid = fork();
        // father in charge of accept
        // child in charge of write
        if(pid == 0)
        {
            close(serv_sock);
            while((str_len = read(clnt_sock, buf, SIZE)) != 0)
            {
                write(clnt_sock, buf, str_len);
                write(fds[1], buf, str_len);
            }
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }
        else 
            close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}