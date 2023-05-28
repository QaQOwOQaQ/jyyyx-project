#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE (32)

void setTime(struct timeval *timeout)
{
    timeout->tv_sec = 3;
    timeout->tv_usec = 0;
}

int main()
{
    fd_set reads, temps;
    int result, str_len, fd_max;
    char buffer[BUF_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads);
    FD_SET(0, &reads);  // fd=0 的是 stdin
    fd_max = 1;

    while(1)
    {
        setTime(&timeout);
        temps = reads;  
        result = select(fd_max+1, &temps, 0, 0, &timeout);
        if(result == -1)
        {
            puts("select error!");
            exit(1);
        }
        else if(result == 0)
        {
            puts("Time-out");
            continue;
        }
        else 
        {
            for(int i = 0; i < fd_max + 1; i ++ )
            {
                if(FD_ISSET(i, &temps))
                {   
                    str_len = read(i, buffer, BUF_SIZE);
                    buffer[str_len] = 0;
                    printf("Message from console: %s", buffer);
                }
            }
        }
    }   
    return 0;
}