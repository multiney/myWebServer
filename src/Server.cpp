#include "./include/Server.h"

#include <functional>

#include "./include/Acceptor.h"
#include "./include/Connection.h"
#include "./include/Socket.h"
#include "./include/ThreadPool.h"
#include "./include/EventLoop.h"

Server::Server(EventLoop *_loop)
    : main_reactor_(_loop), acceptor_(new Acceptor(main_reactor_)) {
    std::function<void(Socket*)> callback = std::bind(&Server::NewConnection, this, std::placeholders::_1);
    acceptor_->SetNewConnCallback(callback);

    int size = static_cast<int>(std::thread::hardware_concurrency());
    thread_pool_ = new ThreadPool(size);
    for (int i = 0; i < size; ++i)
        sub_reactors_.push_back(new EventLoop());

    for (int i = 0; i < size; ++i) {
        std::function<void()> sub_loop = std::bind(&EventLoop::Loop, sub_reactors_[i]);
        thread_pool_->Add(std::move(sub_loop));
    }
}

Server::~Server() {
    delete acceptor_;
    delete thread_pool_;
}

void Server::NewConnection(Socket *sock) {
    uint64_t random = sock->GetFd() % sub_reactors_.size();
    Connection *conn = new Connection(sub_reactors_[random], sock);
    std::function<void(int)> callback = std::bind(&Server::DelConnection, this, std::placeholders::_1);
    conn->SetDelConnectionCallback(callback);
    conn->SetOnConnectionCallback(on_connect_callback_);
    conns_[sock->GetFd()] = conn;
}

void Server::DelConnection(int sockfd) {
    auto iter = conns_.find(sockfd);
    if (iter != conns_.end()) {
        Connection *conn = conns_[sockfd];
        conns_.erase(sockfd);
        delete conn;
        conn = nullptr;
    }
}

void Server::OnConnect(std::function<void (Connection *)> fn) { on_connect_callback_ = std::move(fn); }
