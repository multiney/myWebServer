#include "./include/EventLoop.h"
#include "./include/Channel.h"
#include "./include/Epoll.h"
#include "./include/ThreadPool.h"

#include <vector>

EventLoop::EventLoop() { epoll_ = new Epoll(); }

EventLoop::~EventLoop() { delete epoll_; }

void EventLoop::updateChannel(Channel* ch) { epoll_->updateChannel(ch); }

void EventLoop::loop() {
    std::vector<Channel*> chs; // TODO: inside while?
    while (!quit_) {
        chs = epoll_->poll();
        for (Channel* it : chs)
            it->handleEvent();
    }
}

