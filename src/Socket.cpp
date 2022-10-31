#include "./include/Socket.h"
#include "./include/Util.h"

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

Socket::Socket() {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd_ == -1, "socket create error");
}

Socket::Socket(int fd) : fd_(fd) {
    errif(fd_ == -1, "socket create error");
}

Socket::~Socket() {
    if (fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}

void Socket::bind(InetAddress *addr) {
    struct sockaddr_in tmp_addr = addr->GetAddr();
    errif(::bind(fd_, (sockaddr*)&addr, sizeof(tmp_addr)) == -1, "socket bind error");
}

void Socket::listen() {
    errif(::listen(fd_, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setnonblocking() {
    fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr) {
    struct sockaddr_in tmp_addr{};
    socklen_t addr_len = sizeof(tmp_addr);
    int clnt_sockfd = -1;
    if (fcntl(fd_, F_GETFL) & O_NONBLOCK) {
        while (true) {
            clnt_sockfd = ::accept(fd_, (sockaddr*)&addr, &addr_len);
            if (clnt_sockfd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
                continue;
            } else if (clnt_sockfd == -1) {
                errif(true, "socket accpet error");
            } else {
                break;
            }
        }
    } else {
        clnt_sockfd = ::accept(fd_, (sockaddr*)&addr, &addr_len);
        errif(clnt_sockfd == -1, "socket accpet error");
    }
    addr->SetAddr(tmp_addr);
    return clnt_sockfd;
}

void Socket::connect(InetAddress *addr) {
    struct sockaddr_in tmp_addr = addr->GetAddr();
    if (fcntl(fd_, F_GETFL) & O_NONBLOCK) {
        while (true) {
            int ret = ::connect(fd_, (sockaddr*)&addr, sizeof(tmp_addr));
            if (ret == 0) {
                break;
            } else if (ret == -1 && (errno == EINPROGRESS)) {
                continue;
            /* 连接非阻塞式sockfd建议的做法：
                The socket is nonblocking and the connection cannot be
              completed immediately.  (UNIX domain sockets failed with
              EAGAIN instead.)  It is possible to select(2) or poll(2)
             for completion by selecting the socket for writing.  After
              select(2) indicates writability, use getsockopt(2) to read
              the SO_ERROR option at level SOL_SOCKET to determine
              whether connect() completed successfully (SO_ERROR is
              zero) or unsuccessfully (SO_ERROR is one of the usual
              error codes listed here, explaining the reason for the
              failure).
              这里为了简单、不断连接直到连接完成，相当于阻塞式
            */
            } else if (ret == -1) {
                errif(true, "socket connect error");
            }
        }
    } else {
        errif(::connect(fd_, (sockaddr*)&addr, sizeof(tmp_addr)) == -1, "socket connect error");
    }
}

int Socket::getFd() {
    return fd_;
}

InetAddress::InetAddress() = default;
InetAddress::InetAddress(const char *ip, uint16_t port) {
    memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip);
    addr_.sin_port = htons(port);
}

void InetAddress::SetAddr(sockaddr_in addr) { addr_ = addr; }

sockaddr_in InetAddress::GetAddr() { return addr_; }

const char* InetAddress::GetIp() { return inet_ntoa(addr_.sin_addr); }

uint16_t InetAddress::GetPort() { return ntohs(addr_.sin_port); }
