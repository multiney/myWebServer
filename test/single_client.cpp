#include "src/Util.h"
#include "src/Socket.h"
#include "src/InetAddress.h"
#include "src/Buffer.h"

#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main (int argc, char *argv[])
{
    Socket *sock = new Socket();
    InetAddress *addr= new InetAddress("127.0.0.1", 8888);
    sock->connect(addr);

    Buffer *readBuffer = new Buffer();
    Buffer *sendBuffer = new Buffer();

    while (true) {
        sendBuffer->getLine();
        ssize_t bytes_write = write(sock->getFd(), sendBuffer->c_str(), sendBuffer->size());
        if (bytes_write == -1) {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        int already_read = 0;
        char buf[BUF_SIZE];
        while (true) {
            bzero(&buf, sizeof(buf));
            ssize_t bytes_read = read(sock->getFd(), buf, sizeof(buf));
            if (bytes_read > 0) {
                readBuffer->append(buf);
                already_read += bytes_read;
            } else if (bytes_read == 0) {
                printf("server disconnected!\n");
                exit(EXIT_SUCCESS);
            } else if (bytes_read == -1) {
                close(sock->getFd());
                errif(true, "socket read error");
            }
            if (already_read >= sendBuffer->size()) {// TODO: why if
                printf("message from server: %s\n", readBuffer->c_str());
                break;
            }
        }
        readBuffer->clear();
    }
    delete addr;
    delete sock;
    delete readBuffer;
    delete sendBuffer;
    return 0;
}
