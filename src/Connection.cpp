#include "./include/Connection.h"

#include <unistd.h>

#include <functional>
#include <cstring>
//#include <cassert>
//#include <utility>

#include "./include/Buffer.h"
#include "./include/EventLoop.h"
#include "./include/Channel.h"
#include "./include/Socket.h"

#define BUF_SIZE 1024

Connection::Connection(EventLoop *loop, Socket *sock)
    : loop_(loop), sock_(sock), read_buffer_(new Buffer()), send_buffer_(new Buffer()) {
    if (loop_ != nullptr) {
        channel_ = new Channel(loop_, sock->GetFd());
        channel_->EnableRead();
        channel_->UseET();
    }
    read_buffer_ = new Buffer();
    send_buffer_ = new Buffer();
    state_ = State::Connected;
}

Connection::~Connection() {
    delete sock_;
    if (loop_ != nullptr)
        delete channel_;
    delete read_buffer_;
    delete send_buffer_;
}

void Connection::Read() {
    ASSERT(state_ == State::Connected, "connection state is disconnected");
    read_buffer_->Clear();
    if (sock_->IsNonBlocking())
        ReadNonBlocking();
    else
        ReadBlocking();
}

void Connection::Write() {
    ASSERT(state_ == State::Connected, "connection state is disconnected");
    if (sock_->IsNonBlocking())
        WriteNonBlocking();
    else
        WriteBlocking();
    send_buffer_->Clear();
}



void Connection::ReadNonBlocking() {
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
            break;
        } else if (bytes_read == 0) {
            printf("EOF, client fd %d disconnected\n", sockfd);
            state_ = State::Closed;
            break;
        } else {
            printf("Connection reset by peer\n");
            state_ = State::Closed;
            break;
        }
    }
}

void Connection::WriteNonBlocking() {
    char buf[send_buffer_->Size()];
    std::memcpy(buf, send_buffer_->CStr(), send_buffer_->Size());
    size_t data_size = send_buffer_->Size();
    size_t data_left = data_size;
    while (data_left > 0) {
        ssize_t bytes_write = write(sock_->GetFd(), buf + data_size - data_left, data_left);
        if (bytes_write == -1 && errno == EINTR) {
            printf("continue writing\n");
            continue;
        }
        if (bytes_write == -1 && errno == EAGAIN) {
            break;
        }
        if (bytes_write == -1) {
            printf("Other error on client fd %d\n", sock_->GetFd());
            state_ = State::Closed;
            break;
        }
        data_left -= bytes_write;
    }
}

void Connection::ReadBlocking() {
    unsigned int rcv_size = 0;
    socklen_t len = sizeof(rcv_size);
    getsockopt(sock_->GetFd(), SOL_SOCKET, SO_RCVBUF, &rcv_size, &len);
    char buf[rcv_size];
    ssize_t bytes_read = read(sock_->GetFd(), buf, sizeof(buf));
    if (bytes_read > 0) {
        read_buffer_->Append(buf);
    } else if (bytes_read == 0) {
        printf("read EOF, blocking client fd %d disconnected\n", sock_->GetFd());
        state_ = State::Closed;
    } else if (bytes_read == -1) {
        printf("Other error on blocking client fd %d\n", sock_->GetFd());
        state_ = State::Closed;
    }
}

void Connection::WriteBlocking() {
    int sockfd = sock_->GetFd();
    ssize_t bytes_write = write(sockfd, send_buffer_->CStr(), send_buffer_->Size());
    if (bytes_write == -1) {
        printf("Other error on blocking client fd %d\n", sockfd);
        state_ = State::Closed;
    }
}

void Connection::Close() { del_connection_callback_(sock_->GetFd()); }

Connection::State Connection::GetState() { return state_; }
void Connection::SetSendBuffer(const char *str) { send_buffer_->SetBuf(str); }
Buffer *Connection::GetReadBuffer() { return read_buffer_; }
const char *Connection::ReadBuffer() { return read_buffer_->CStr(); }
Buffer *Connection::GetSendBuffer() { return send_buffer_; }
const char *Connection::SendBuffer() { return send_buffer_->CStr(); }

void Connection::SetDelConnectionCallback(std::function<void(int)> const &callback) {
    del_connection_callback_ = callback;
}

void Connection::SetOnConnectionCallback(const std::function<void (Connection *)> &callback) {
    on_connect_callback_ = callback;
    channel_->SetReadCallback([this](){ on_connect_callback_(this); });
}

void Connection::GetLineSendBuffer() { send_buffer_->GetLine(); }

Socket *Connection::GetSocket() { return sock_; }
