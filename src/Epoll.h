#pragma once

class Epoll
{
private:
    int epfd;
    struct epoll_event *events;
public:
    Epoll();
    ~Epoll();
};
