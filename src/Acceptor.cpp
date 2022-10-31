#include "./include/Acceptor.h"

#include <functional>

#include "./include/EventLoop.h"
#include "./include/Socket.h"
#include "./include/Channel.h"


Acceptor::Acceptor(EventLoop *loop)
    : loop_(loop), sock_(new Socket()), accept_channel_(new Channel(loop_, sock_->GetFd())) { // should be initialized in a member initializer of the constructor
    InetAddress *addr = new InetAddress("127.0.0.1", 8888);
    sock_->Bind(addr);
    sock_->Listen();
    //sock->setnonblocking();
    // accept_channel_ = new Channel(loop_, sock_->getFd());
    std::function<void()> callback = std::bind(&Acceptor::AcceptConn, this);
    accept_channel_->SetReadCallback(callback);
    accept_channel_->EnableRead();
    delete addr;
}

Acceptor::~Acceptor() {
    delete sock_;
    delete accept_channel_;
}

void Acceptor::SetNewConnCallback(std::function<void (Socket*)> const &callback) {
    new_connection_callback_ = callback;
}

void Acceptor::AcceptConn() {
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(sock_->Accept(clnt_addr));
    printf("new client fd %d IP: %s Port: %d\n", clnt_sock->GetFd(), inet_ntoa(clnt_addr->GetAddr().sin_addr), ntohs(clnt_addr->GetAddr().sin_port));
    clnt_sock->Setnonblocking();
    new_connection_callback_(clnt_sock);
    delete clnt_addr;
}
