#pragma once

#include <map>
#include <vector>

class EventLoop;
class Acceptor;
class Connection;
class Socket;
class ThreadPool;
class Server
{
private:
    EventLoop *main_reactor_;
    Acceptor *acceptor_;
    std::map<int, Connection*> conns_;
    std::vector<EventLoop*> sub_reactors_;
    ThreadPool *thread_pool_;
public:
    explicit Server(EventLoop *loop);
    ~Server();

    void NewConnection(Socket *sock);
    void DelConnection(int sockfd);
};
