#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE (32)

/* 偷懒，就不写 error_handling() */

int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        printf("Usage : <GroupIp> <Port>\n");
        exit(1);
    }
    int send_sock;
    struct sockaddr_in broad_addr;
    FILE *fp;
    int so_brd = 1; // set socket
    char buffer[BUF_SIZE];

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&broad_addr, 0, sizeof(broad_addr));
    broad_addr.sin_family = AF_INET;
    broad_addr.sin_addr.s_addr = inet_addr(argv[1]);
    broad_addr.sin_port = htons(atoi(argv[2]));
    
    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST,
            (void*)&so_brd, sizeof(so_brd));
    fp = fopen("message.txt", "r");
    
    while(1)
    {
        fgets(buffer, BUF_SIZE, fp);
        if(feof(fp))    break;
        sendto(send_sock, buffer, strlen(buffer), 0,
            (struct sockaddr*)&broad_addr, sizeof(broad_addr));
        sleep(2);
    }
    fclose(fp);
    close(send_sock);
    return 0;
}

