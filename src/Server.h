#pragma once

#include <map>

class EventLoop;
class Acceptor;
class Connection;
class Socket;
class Server
{
private:
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int, Connection*> conns;
public:
    Server(EventLoop *_loop);
    ~Server();

    void newConnection(Socket*);
    void delConnection(Socket*);
};
