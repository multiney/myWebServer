#include "./include/Acceptor.h"
#include "./include/EventLoop.h"
#include "./include/Socket.h"
#include "./include/Channel.h"
#include "./include/Util.h"

#include <functional>
#include <string.h>
#include <sys/socket.h>

Acceptor::Acceptor(EventLoop *_loop) : loop(_loop), sock(nullptr), acceptChannel(nullptr) {
    sock = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 8888);
    sock->bind(addr);
    sock->listen();
    //sock->setnonblocking();
    acceptChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConn, this);
    acceptChannel->setReadCallback(cb);
    acceptChannel->enableRead();
    delete addr;
}

Acceptor::~Acceptor() {
    delete sock;
    delete acceptChannel;
}

void Acceptor::setNewConnCallback(std::function<void (Socket*)> const &callback) {
    newConnCB = callback;
}

void Acceptor::acceptConn() {
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(sock->accept(clnt_addr));
    printf("new client fd %d IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->GetAddr().sin_addr), ntohs(clnt_addr->GetAddr().sin_port));
    clnt_sock->setnonblocking();
    newConnCB(clnt_sock);
    delete clnt_addr;
}
