#pragma once

#include <functional>

class EventLoop;
class Socket;
class Channel;
class Acceptor
{
public:
    explicit Acceptor(EventLoop *loop);
    ~Acceptor();

    void SetNewConnCallback(std::function<void(Socket*)> const &callback); //TODO: void(int)?
    void AcceptConn();
private:
    EventLoop *loop_{nullptr};
    Socket *sock_{nullptr};
    Channel *accept_channel_{nullptr};
    std::function<void(Socket*)> new_connection_callback_;
};
