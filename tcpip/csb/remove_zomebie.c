#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status))
    {
        printf("Remove proc id: %d\n", id);
        printf("Child send: %d\n", WEXITSTATUS(status));
    }
}

int main()
{
    pid_t pid;
    
    // 初始化 sigaction
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    // fork
    pid = fork();
    if(pid == 0) // child_1
    {
        puts("Hi I'm child process");
        sleep(10);
        return 12;
    }
    else  // father
    {
        printf("Child proc id: %d\n", pid);
        pid = fork();
        if(pid == 0) // child_2
        {
            puts("Hi I'm child process");
            sleep(10);
            return 24;
        }
        else // father 
        {
            printf("Child proc id: %d\n", pid);
            for(int i = 0; i < 5; i ++ )
            {
                puts("wait...");
                sleep(5);
            }
        }
    }

    return 0;
}