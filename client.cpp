#include <cstdlib>
#include <iostream>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "src/util.h"
#include "src/Buffer.h"
#include "src/Socket.h"
#include "src/InetAddress.h"

#define BUF_SIZE 1024

int main (int argc, char *argv[])
{
    Socket *sock = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 8888);
    sock->connect(addr);

    int sockfd = sock->getFd();

    Buffer *sendBuffer = new Buffer();
    Buffer *readBuffer = new Buffer();

    while (true) {
        sendBuffer->getline();
        ssize_t write_bytes = write(sockfd, sendBuffer->c_str(), sendBuffer->size());
        if (write_bytes == -1) {
            printf("socket already disconnected, can't write any more\n");
            break;
        }
        int already_read = 0;
        char buf[BUF_SIZE];
        while (true) {
            bzero(&buf, sizeof(buf));
            ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
            if (read_bytes > 0) {
                readBuffer->append(buf, sizeof(buf));
                already_read += read_bytes;
            } else if (read_bytes == 0) {
                printf("server disconnected!\n");
                exit(EXIT_SUCCESS);
            }
            if (already_read >= sendBuffer->size()) {
                printf("message from server: %s\n", readBuffer->c_str());
                break;
            }
        }
        readBuffer->clear();
    }
    delete addr;
    delete sock;
    return 0;
}
