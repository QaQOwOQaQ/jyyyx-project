#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(const char *message);

int main(int argc, char **argv)
{
    struct hostent *host;
    if(argc != 2)
    {
        printf("Usage : <addr>\n");
        exit(1);
    }

    // 1. get hostent
        // 1.1 by domin name    
    if(argv[1][0] == 'w')
    {
        printf("ByName()\n");
        host = gethostbyname(argv[1]);
    }
    else 
    {
            // 1.2 by IP addr
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_addr.s_addr = inet_addr(argv[1]);
        host = gethostbyaddr((char *)&addr.sin_addr, 4, AF_INET);
    }

    if(host == NULL)
        error_handling("gethostbyname() error");
    
    // 1. offical host name
    printf("Official host name: %s\n", host->h_name);
    // 2. other host names
    for(int i = 0; host->h_aliases[i]; i ++ )
        printf("Aliases %d: %s\n", i + 1, host->h_aliases[i]);
    // 3. netword layer addr type
    printf("Address type: %s\n", (host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6")); 
    // 4. newwork layer addr length
    printf("Address length: %d\n", host->h_length);
    // 5. network layer addr 
    for(int i = 0; host->h_addr_list[i]; i ++ )
    printf("IP addr %d: %s\n", i + 1, 
            inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    // 6. test
    /* 像下面这样输出会输出乱码
        因为 host->h_addr_llist 指向的是 struct in_addr 的地址
    */
    for(int i = 0; host->h_addr_list[i]; i ++ )
    printf("IP addr %d: %s\n", i + 1,  host->h_addr_list[i]);
    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

static void entry()
{
    struct hostent *gethostbyname(const char *hostname);
    struct hostent
    {
        char *h_name;
        char **h_aliases;
        int h_addrtype;
        int h_length;
        char **h_addr_list;
    };
}



