#include "./include/EventLoop.h"

#include <vector>

#include "./include/Channel.h"
#include "./include/Epoll.h"

EventLoop::EventLoop() : epoll_(new Epoll()) { }

EventLoop::~EventLoop() { delete epoll_; }

void EventLoop::UpdateChannel(Channel* channel) { epoll_->UpdateChannel(channel); }

void EventLoop::Loop() {
    std::vector<Channel*> chs; // TODO: inside while?
    while (!quit_) {
        chs = epoll_->Poll();
        for (Channel* channel : chs)
            channel->HandleEvent();
    }
}

