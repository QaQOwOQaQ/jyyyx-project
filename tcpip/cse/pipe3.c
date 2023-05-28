// 使用两个管道实现双向通信
#include <stdio.h>
#include <unistd.h>

#define SIZE (32)

int main()
{
    int fds1[2], fds2[2];
    char str1[] = "🐱好可爱";
    char str2[] = "🐶好可爱";
    char buf[SIZE];
    pid_t pid;

    pipe(fds1);
    pipe(fds2);
    pid = fork();
    if(pid == 0)
    {
        write(fds1[1], str1, sizeof(str1));
        read(fds2[0], buf, SIZE);
        printf("Child proc output: %s\n", buf);
    }
    else 
    {
        read(fds1[0], buf, SIZE);
        printf("Parent proc output: %s\n", buf);
        write(fds2[1], str2, sizeof(str2));
    }
}