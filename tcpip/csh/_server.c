// name sender is better than server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL (64)
#define SIZE (32)

int main(int argc, char **argv)
{
    int send_sock;
    struct sockaddr_in mul_addr;
    int time_live = TTL;
    FILE *fp;
    char buf[SIZE];
    if(argc != 2)
    {
        printf("Usage : <Port>\n");
        exit(1);
    }
    send_sock = socket(PF_INET, SOCK_DGRAM, 0); // udp

    memset(&mul_addr, 0, sizeof(mul_addr));
    mul_addr.sin_addr.s_addr = htonl(INADDR_ANY);   // let local ip addr to multicast group ip addr
    mul_addr.sin_family = AF_INET;
    mul_addr.sin_port = htons(atoi(argv[1]));
    
    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, 
                (void*)&time_live, sizeof(time_live));
    fp = fopen("message", "r");    // unix 

    while(1)
    {
        fgets(buf, SIZE, fp);
        if(feof(fp))    break;
        sendto(send_sock, buf, strlen(buf), 0,
                (struct sockaddr *)&mul_addr, sizeof(mul_addr));
        sleep(2);
    }
    fclose(fp);
    close(send_sock);
    return 0;
}   