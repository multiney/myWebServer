#include "./include/Server.h"
#include "./include/Acceptor.h"
#include "./include/Connection.h"
#include "./include/Socket.h"
#include "./include/ThreadPool.h"
#include "./include/EventLoop.h"

#include <functional>

Server::Server(EventLoop *_loop)
    : mainReactor(_loop), acceptor(nullptr) {
    acceptor = new Acceptor(mainReactor);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnCallback(cb);

    int size = static_cast<int>(std::thread::hardware_concurrency());
    thpool = new ThreadPool(size);
    for (int i = 0; i < size; ++i)
        subReactors.push_back(new EventLoop());

    for (int i = 0; i < size; ++i) {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactors[i]);
        thpool->add(std::move(sub_loop));
    }
}

Server::~Server() {
    delete acceptor;
    delete thpool;
}

void Server::newConnection(Socket *sock) {
    uint64_t random = sock->getFd() % subReactors.size();
    Connection *conn = new Connection(subReactors[random], sock);
    std::function<void(int)> cb = std::bind(&Server::delConnection, this, std::placeholders::_1);
    conn->setDelConnectionCallback(cb);
    conns[sock->getFd()] = conn;
}

void Server::delConnection(int sockfd) {
    auto it = conns.find(sockfd);
    if (it != conns.end()) {
        Connection *conn = conns[sockfd];
        conns.erase(sockfd);
        delete conn;
    }
}
