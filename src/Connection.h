#pragma once

class EventLoop;
class Channel;
class Connection
{
private:
    Channel *channel;
    int fd;
    EventLoop *loop;
public:
    Connection(EventLoop *_loop, int _fd);
    ~Connection();
};
