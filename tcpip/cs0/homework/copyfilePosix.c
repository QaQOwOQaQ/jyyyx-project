#include <stdio.h>
#include <string.h>

#define SIZE (1024)

char *buf[SIZE];

int main()
{
    FILE *readfile = fopen("src2.txt", "r");
    FILE *writefile = fopen("dst2.txt", "a");
    fread(buf, SIZE, 1, readfile);
    fwrite(buf, SIZE, 1, writefile);
    fclose(readfile);
    fclose(writefile);
    return 0;
}