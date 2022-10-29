#pragma once

#include <functional>

class EventLoop;
class Channel
{
private:
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t ready;
    bool inEpoll;
    bool useThreadPool;
    std::function<void()> readCallback;
    std::function<void()> writeCallback;
public:
    Channel(EventLoop *_loop, int sockfd);
    ~Channel();

    void enableRead();
    void handleEvent();

    int getFd();
    uint32_t getEvents();
    uint32_t getReady();
    bool getInEpoll();

    void setReady(uint32_t);
    void setInEpoll(bool _in = true);
    void setUseThreadPool(bool use = true);
    void useET();
    void setReadCallback(std::function<void()> _cb);
};
