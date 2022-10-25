#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"

#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false) {
    ep = new Epoll();
}

EventLoop::~EventLoop() {
    delete ep;
}

void EventLoop::updateChannel(Channel* channel) {
    ep->updateChannel(channel);
}

void EventLoop::loop() {
    std::vector<Channel*> chs;
    while (!quit) {
        chs = ep->poll();
        for (Channel* it : chs)
            it->handleEvent();
    }
}
