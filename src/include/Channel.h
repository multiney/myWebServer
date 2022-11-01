#pragma once

#include <functional>

class EventLoop;
class Channel {
 private:
    EventLoop *loop_;
    int fd_;
    uint32_t events_;
    uint32_t ready_;
    bool in_epoll_;
    std::function<void()> read_callback_;
    std::function<void()> write_callback_;

 public:
    Channel(EventLoop *loop, int sockfd);
    ~Channel() = default;

    void EnableRead();
    void HandleEvent();

    int GetFd();
    uint32_t GetEvents();
    uint32_t GetReady();
    bool GetInEpoll();

    void SetReady(uint32_t ready);
    void SetInEpoll(bool inEpoll = true);
    void UseET();
    void SetReadCallback(std::function<void()> const &callback);
};
