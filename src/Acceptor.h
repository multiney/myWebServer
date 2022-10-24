#pragma once

class EventLoop;
class Socket;
class InetAddress;
class Acceptor
{
private:
    EventLoop *loop;
    Socket *sock;
    InetAddress *addr;
public:
    Acceptor(EventLoop*);
    ~Acceptor();
};
