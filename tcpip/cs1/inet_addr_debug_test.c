#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    char *s = "1.2.3.4";
    struct sockaddr_in addr;
    addr.sin_addr.s_addr = inet_addr(s);
    printf("%x\n", addr.sin_addr.s_addr);   // 4030201
    printf("%s\n", inet_ntoa(addr.sin_addr));   // 1.2.3.4
    addr.sin_addr.s_addr = htonl(inet_addr(s));
    printf("%x\n", addr.sin_addr.s_addr);   // 1020304
    printf("%s\n", inet_ntoa(addr.sin_addr));   // 1.2.3.4
    
    return 0;
}