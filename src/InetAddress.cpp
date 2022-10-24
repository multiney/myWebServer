#include "InetAddress.h"

#include <arpa/inet.h>
#include <strings.h>
#include <sys/socket.h>

InetAddress::InetAddress() {
    bzero(&addr, sizeof(addr));
}

InetAddress::InetAddress(const char *ip, uint16_t port) {
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(port);
}

InetAddress::~InetAddress() {

}

void InetAddress::setInetAddress(sockaddr_in _addr) {
    addr = _addr;
}

sockaddr_in InetAddress::getAddr() {
    return addr;
}
