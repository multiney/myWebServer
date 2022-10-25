#include "Connection.h"
#include "Buffer.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"
#include "Util.h"

#include <functional>
#include <strings.h>
#include <unistd.h>

#define BUF_SIZE 1024

Connection::Connection(EventLoop *_loop, Socket *_sock) : loop(_loop), sock(_sock), channel(nullptr), readBuffer(nullptr) {
    channel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this);
    channel->setCallback(cb);
    channel->enableRead();
    readBuffer = new Buffer();
}

Connection::~Connection() {
    delete sock;
    delete channel;
    delete readBuffer;
}

void Connection::setDelConnectionCallback(std::function<void(Socket*)> _cb) {
    delConnCallback = _cb;
}

void Connection::echo() {
    char buf[BUF_SIZE];
    int fd = sock->getFd();
    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(fd, buf, sizeof(buf));
        if (bytes_read > 0) {
            readBuffer->append(buf);
        } else if (bytes_read == -1 && errno == EINTR) {
            printf("continue reading\n");
            continue;
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            printf("finish reading once, errno: %d\n", errno);
            printf("message from client fd %d : %s\n", fd, readBuffer->c_str());
            errif(write(fd, readBuffer->c_str(), readBuffer->size()) == -1, "socket write error");
            readBuffer->clear();
            break;
        } else if (bytes_read == 0) {
            printf("EOF, client fd %d disconnected\n", fd);
            delConnCallback(sock);
            break;
        }
    }
}
