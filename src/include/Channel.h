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
    std::function<void()> readCallback;
    std::function<void()> writeCallback;
public:
    Channel(EventLoop *loop, int sockfd);
    ~Channel();

    void enableRead();
    void handleEvent();

    int getFd();
    uint32_t getEvents();
    uint32_t getReady();
    bool getInEpoll();

    void setReady(uint32_t ev);
    void setInEpoll(bool in = true);
    void useET();
    void setReadCallback(std::function<void()> const &callback);
};
