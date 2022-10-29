#pragma once

#include <functional>

class Epoll;
class Channel;
class ThreadPool;
class EventLoop
{
private:
    Epoll *ep;
    ThreadPool *threadPool;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void updateChannel(Channel*);
    void loop();
    void addThread(std::function<void()> func);
};
