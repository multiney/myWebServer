#pragma once

#include <functional>

class Epoll;
class Channel;
class EventLoop
{
private:
    Epoll *epoll_{nullptr};
    bool quit_{false};
public:
    EventLoop();
    ~EventLoop();

    void updateChannel(Channel *ch);
    void loop();
};
