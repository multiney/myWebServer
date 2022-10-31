#pragma once

#include <functional>

class EventLoop;
class Socket;
class Channel;
class Acceptor
{
private:
    EventLoop *loop;
    Socket *sock;
    Channel *acceptChannel;
    std::function<void(Socket*)> newConnCB;
public:
    Acceptor(EventLoop *loop);
    ~Acceptor();

    void setNewConnCallback(std::function<void(Socket*)> const &callback); //TODO: void(int)?
    void acceptConn();
};
