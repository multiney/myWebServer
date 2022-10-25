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
    std::function<void()> callback;
public:
    Channel(EventLoop *_loop, int sockfd);
    ~Channel();

    void setCallback(std::function<void()> _cb);
    void enableRead();
    void handleEvent();

    int getFd();
    uint32_t getEvents();
    uint32_t getReady();
    void setReady(uint32_t);
    bool getInEpoll();

    void setInEpoll();
};
