#pragma once

#include <functional>

class EventLoop;
class Channel;
class Socket;
class Buffer;
class Connection
{
private:
    EventLoop *loop_;
    Socket *sock_;
    Channel *channel_;
    Buffer *read_buffer_;
    std::function<void(int)> del_connection_callback_;
public:
    Connection(EventLoop *_loop, Socket *_sock);
    ~Connection();

    void SetDelConnectionCallback(std::function<void(int)> const &callback);
    void Echo();
    void Send();
};
