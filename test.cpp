#include <stdio.h>
#include <string.h>
#include <strings.h>

int main (int argc, char *argv[])
{
    char buf[10];
    bzero(buf, 10);
    scanf("%s", buf);
    printf("%s\n", buf);
    return 0;
}
