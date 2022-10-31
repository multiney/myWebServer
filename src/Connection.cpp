#include "./include/Connection.h"
#include "./include/Buffer.h"
#include "./include/EventLoop.h"
#include "./include/Channel.h"
#include "./include/Socket.h"
#include "./include/Util.h"

#include <cstring>
#include <functional>
#include <strings.h>
#include <unistd.h>

#define BUF_SIZE 1024

Connection::Connection(EventLoop *_loop, Socket *_sock)
    : loop(_loop), sock(_sock), channel(nullptr), readBuffer(nullptr) {
    channel = new Channel(loop, sock->getFd());
    channel->enableRead();
    channel->useET();
    std::function<void()> cb = std::bind(&Connection::echo, this);
    channel->setReadCallback(cb);
    //channel->setUseThreadPool(true);
    readBuffer = new Buffer();
}

Connection::~Connection() {
    delete sock;
    delete channel;
    delete readBuffer;
}

void Connection::setDelConnectionCallback(std::function<void(int)> const &callback) {
    delConnCallback = callback;
}

void Connection::echo() {
    char buf[BUF_SIZE];
    int fd = sock->getFd();
    while (true) {
        memset(&buf, 0, sizeof(buf));
        ssize_t bytes_read = read(fd, buf, sizeof(buf));
        if (bytes_read > 0) {
            readBuffer->append(buf);
        } else if (bytes_read == -1 && errno == EINTR) {
            printf("continue reading\n");
            continue;
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            //printf("finish reading once, errno: %d\n", errno);
            printf("message from client fd %d : %s\n", fd, readBuffer->c_str());
            //errif(write(fd, readBuffer->c_str(), readBuffer->size()) == -1, "socket write error");
            send();
            readBuffer->clear();
            break;
        } else if (bytes_read == 0) {
            printf("EOF, client fd %d disconnected\n", fd);
            delConnCallback(fd);
            break;
        } else {
            printf("Connection reset by peer\n");
            delConnCallback(fd);
            break;
        }
    }
}

void Connection::send() {
    char buf[readBuffer->size()];
    strcpy(buf, readBuffer->c_str());
    int data_size = readBuffer->size();
    int data_left = data_size;
    while (data_left > 0) {
        ssize_t bytes_write = write(sock->getFd(), buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EAGAIN)
            break;
        data_left -= bytes_write;
    }
}
