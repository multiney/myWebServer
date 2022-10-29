#include "Server.h"
#include "Acceptor.h"
#include "Connection.h"
#include "Socket.h"

#include <functional>

Server::Server(EventLoop *_loop) : loop(_loop), acceptor(nullptr) {
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnCallback(cb);
}

Server::~Server() {
    delete acceptor;
}

void Server::newConnection(Socket *sock) {
    Connection *conn = new Connection(loop, sock);
    std::function<void(int)> cb = std::bind(&Server::delConnection, this, std::placeholders::_1);
    conn->setDelConnectionCallback(cb);
    conns[sock->getFd()] = conn;
}

void Server::delConnection(int sockfd) {
    if (sockfd != -1) {
        auto it = conns.find(sockfd);
        if (it != conns.end()) {
            Connection *conn = conns[sockfd];
            conns.erase(sockfd);
            delete conn;
        }
    }
}
