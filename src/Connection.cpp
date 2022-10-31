#include "./include/Connection.h"

#include <unistd.h>

#include <functional>
#include <cstring>

#include "./include/Buffer.h"
#include "./include/EventLoop.h"
#include "./include/Channel.h"
#include "./include/Socket.h"

#define BUF_SIZE 1024

Connection::Connection(EventLoop *loop, Socket *sock)
    : loop_(loop), sock_(sock), channel_(new Channel(loop, sock->GetFd())), read_buffer_(new Buffer()) {
    channel_->EnableRead();
    channel_->UseET();
    std::function<void()> callback = std::bind(&Connection::Echo, this);
    channel_->SetReadCallback(callback);
}

Connection::~Connection() {
    delete sock_;
    delete channel_;
    delete read_buffer_;
}

void Connection::SetDelConnectionCallback(std::function<void(int)> const &callback) {
    del_connection_callback_ = callback;
}

void Connection::Echo() {
    char buf[BUF_SIZE];
    int sockfd = sock_->GetFd();
    while (true) {
        std::memset(&buf, 0, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            read_buffer_->Append(buf);
        } else if (bytes_read == -1 && errno == EINTR) {
            printf("continue reading\n");
            continue;
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            //printf("finish reading once, errno: %d\n", errno);
            printf("message from client fd %d : %s\n", sockfd, read_buffer_->CStr());
            //errif(write(fd, readBuffer->c_str(), readBuffer->size()) == -1, "socket write error");
            Send();
            read_buffer_->Clear();
            break;
        } else if (bytes_read == 0) {
            printf("EOF, client fd %d disconnected\n", sockfd);
            del_connection_callback_(sockfd);
            break;
        } else {
            printf("Connection reset by peer\n");
            del_connection_callback_(sockfd);
            break;
        }
    }
}

void Connection::Send() {
    char buf[read_buffer_->Size()];
    std::strcpy(buf, read_buffer_->CStr());
    size_t data_size = read_buffer_->Size();
    size_t data_left = data_size;
    while (data_left > 0) {
        ssize_t bytes_write = write(sock_->GetFd(), buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EAGAIN) {
            break;
        }
        data_left -= bytes_write;
    }
}
