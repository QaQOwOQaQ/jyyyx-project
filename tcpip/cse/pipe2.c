// create a pipe to let child and father 
// communicate double-slided
#include <stdio.h>
#include <unistd.h>

#define SIZE (100)

int main()
{
    int fds[2];
    char str1[SIZE] = "😄笑嘻嘻了";
    char str2[SIZE] = "😭哭唧唧了";
    char buf[SIZE];
    pid_t pid;

    pipe(fds); // create pipe
    pid = fork();
    if(pid == 0)
    {
        // sleep(5);
        write(fds[1], str1, sizeof(str1));
        sleep(2);   // must, otherwise next read will clear pile, and father's read will dead wait
                    //  sleep can make father run
        read(fds[0], buf, SIZE);
        printf("Child proc output: %s\n", buf);
        return 0;
    }
    else 
    {
        read(fds[0], buf, SIZE);
        printf("Parent proc output: %s\n", buf);
        write(fds[1], str2, sizeof(str2));
        sleep(3);   // if father not sleep, father will done and child process will conversion to 孤儿 process
                        // and managed by init process, when father done, after times, child done
    }
    return 0;
}