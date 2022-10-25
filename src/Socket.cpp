#include "Socket.h"
#include "Util.h"
#include "InetAddress.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

Socket::Socket() :fd(-1) {
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
    errif(::bind(fd, (sockaddr*)&addr, sizeof(addr)) == -1, "socket bind error");
}

void Socket::listen() {
    errif(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}

void Socket::setnonblocking() {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *_addr) {
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr = _addr->getAddr();
    socklen_t addr_len = sizeof(addr);
    int clnt_sockfd = ::accept(fd, (sockaddr*)&addr, &addr_len);
    errif(clnt_sockfd == -1, "socket accpet error");
    _addr->setInetAddress(addr);
    return clnt_sockfd;
}

void Socket::connect(InetAddress *_addr) {
    struct sockaddr_in addr = _addr->getAddr();
    errif(::connect(fd, (sockaddr*)&addr, sizeof(addr)) == -1, "socket connect error");
}

int Socket::getFd() {
    return fd;
}
