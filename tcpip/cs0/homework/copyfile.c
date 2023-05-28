#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE (1024)

char buf[BUFFER_SIZE];

int main()
{
    int fd = open("copysrc.txt", O_RDONLY); // <fcntl.h>
    if(fd == -1)
    {
        printf("open() error");
        exit(1);
    }
    
    if(read(fd, buf, sizeof(buf)) == -1)
    {
        printf("open() error");
        exit(1);
    }
    printf("file data: %s", buf);
    
    int newfd = open("copydst.txt", O_CREAT | O_WRONLY);
    if(write(newfd, buf, strlen(buf)) == -1)    // if use sizeof will write NULL... into file 
    {
        printf("open() error");
        exit(1);
    }
    close(fd);
    close(newfd);

    return 0;
}