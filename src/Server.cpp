#include "Server.h"
#include "Acceptor.h"
#include "Connection.h"

Server::Server(EventLoop *_loop) : loop(_loop), acceptor(nullptr) {
    acceptor = new Acceptor(loop);
}

Server::~Server() {

}

void Server::newConnection(int sockfd) {
    Connection *conn = new Connection(loop, sockfd);
}
