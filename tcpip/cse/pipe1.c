// create a pipe to let child write to father
#include <stdio.h>
#include <unistd.h>

int main()
{
    int fds[2];
    char str[] = "who jb you❓";
    char buf[1024];

    pid_t pid;
    pipe(fds);  // create pipe
    pid = fork();
    if(pid == 0)
    {
        write(fds[1], str, sizeof(str));
    }
    else 
    {
        read(fds[0], buf, 1024);
        puts(buf);
    }

    return 0;
}