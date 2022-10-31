#pragma once

#include <functional>

class EventLoop;
class Channel;
class Socket;
class Buffer;
class Connection
{
private:
    Channel *channel;
    Socket *sock;
    EventLoop *loop;
    std::function<void(int)> delConnCallback;
    Buffer *readBuffer;
public:
    Connection(EventLoop *_loop, Socket *_sock);
    ~Connection();

    void setDelConnectionCallback(std::function<void(int)> const &callback);
    void echo();
    void send();
};
