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
    EventLoop *mainReactor;
    Acceptor *acceptor;
    std::map<int, Connection*> conns;
    std::vector<EventLoop*>subReactors;
    ThreadPool *thpool;
public:
    explicit Server(EventLoop *loop);
    ~Server();

    void newConnection(Socket *sock);
    void delConnection(int sockfd);
};
