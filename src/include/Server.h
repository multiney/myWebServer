#pragma once

#include <functional>
#include <map>
#include <vector>
#include "Macros.h"

class EventLoop;
class Acceptor;
class Connection;
class Socket;
class ThreadPool;
class Server {
 private:
    EventLoop *main_reactor_;
    Acceptor *acceptor_;
    std::map<int, Connection*> conns_;
    std::vector<EventLoop*> sub_reactors_;
    ThreadPool *thread_pool_;
    std::function<void(Connection *)> on_connect_callback_;

 public:
    explicit Server(EventLoop *loop);
    ~Server();

    DISALLOW_COPY_AND_MOVE(Server);

    void NewConnection(Socket *sock);
    void DelConnection(int sockfd);
    void OnConnect(std::function<void(Connection *)> fn);
};
