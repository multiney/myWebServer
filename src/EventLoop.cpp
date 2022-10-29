#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"
#include "ThreadPool.h"

#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false) {
    ep = new Epoll();
    threadPool = new ThreadPool();
}

EventLoop::~EventLoop() {
    delete threadPool;
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

void EventLoop::addThread(std::function<void ()> func) {
    threadPool->add(func);
}
