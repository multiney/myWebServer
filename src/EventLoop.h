#pragma once

class Epoll;
class EventLoop
{
private:
    Epoll *ep;
    bool quit;
public:
    EventLoop();
    ~EventLoop();
};
