#include <iostream>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

#define BUF_SIZE 1024

int main (int argc, char *argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "socket create error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket connect error");

    char buf[BUF_SIZE];
    while (true) {
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, strlen(buf));
        if (write_bytes == -1) {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        ssize_t read_bytes = read(sockfd, buf, write_bytes);
        buf[read_bytes] = 0;
        if (read_bytes > 0)
            printf("message from server: %s\n", buf);
        else if (read_bytes == 0) {
            printf("server socket disconnected!\n");
            break;
        }
        else if (read_bytes == -1) {
            close(sockfd);
            errif(true, "socket read error");
        }
    }
    close(sockfd);
    return 0;
}
