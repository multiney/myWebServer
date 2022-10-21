#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <thread>

int main (int argc, char *argv[])
{
    char buf[10];
    bzero(buf, 10);
    scanf("%s", buf);
    printf("%s\n", buf);
    std::thread::hardware_concurrency();
    return 0;
}
