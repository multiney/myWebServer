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
    std::function<void(Socket*)> cb = std::bind(&Server::delConnection, this, std::placeholders::_1);
    conn->setDelConnectionCallback(cb);
    conns[sock->getFd()] = conn;
}

void Server::delConnection(Socket *sock) {
    Connection *conn = conns[sock->getFd()];
    conns.erase(sock->getFd());
    delete conn;
}
