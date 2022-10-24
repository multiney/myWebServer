#pragma once

#include <cstdint>
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
    std::function<void(int)> callback;
public:
    Channel(EventLoop *_loop, int sockfd);
    ~Channel();
};
