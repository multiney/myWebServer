#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "Socket.h"
#include "InetAddress.h"
#include "util.h"

Socket::Socket() : fd(-1) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error");
}

Socket::Socket(int _fd) : fd(_fd) {
    errif(fd == -1, "socket create error");
}

Socket::~Socket() {
    if (fd != -1) {
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress *_addr) {
    struct sockaddr_in addr = _addr->getAddr();
    socklen_t addr_len = _addr->getAddr_len();
    errif(::bind(fd, (sockaddr*)&addr, addr_len) == -1, "socket bind error");
    _addr->setInetAddr(addr, addr_len); // TODO: why do that?
}

void Socket::listen() {
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setnonblocking() {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *_addr) {
    struct sockaddr_in addr = _addr->getAddr();
    socklen_t addr_len = _addr->getAddr_len();
    int clnt_sockfd = ::accept(fd, (sockaddr*)&addr, &addr_len);
    errif(clnt_sockfd == -1, "socket accept error");
    _addr->setInetAddr(addr, addr_len);
    return clnt_sockfd;
}

void Socket::connect(InetAddress *_addr) {
    struct sockaddr_in addr = _addr->getAddr(); // TODO: '_addr->getAddr()' why temporary?
    socklen_t addr_len = _addr->getAddr_len();
    errif(::connect(fd, (sockaddr*)&addr, addr_len) == -1, "socket connect error");
}

int Socket::getFd() {
    return fd;
}
