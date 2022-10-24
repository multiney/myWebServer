#include "EventLoop.h"
#include "Epoll.h"

EventLoop::EventLoop() : ep(nullptr), quit(false) {
    ep = new Epoll();
}

EventLoop::~EventLoop() {
    
}
