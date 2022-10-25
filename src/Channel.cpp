#include "Channel.h"
#include "EventLoop.h"

#include <sys/epoll.h>
#include <unistd.h>

Channel::Channel(EventLoop *_loop, int _fd) : loop(_loop), fd(_fd), events(0), ready(0), inEpoll(false) {

}

Channel::~Channel() {
}

void Channel::setCallback(std::function<void ()> _cb) {
    callback = _cb;
}

void Channel::enableRead() {
    events |= EPOLLET | EPOLLIN;
    loop->updateChannel(this);
}

void Channel::handleEvent() {
    callback();
}

int Channel::getFd() {
    return fd;
}

uint32_t Channel::getEvents() {
    return events;
}

uint32_t Channel::getReady() {
    return ready;
}

void Channel::setReady(uint32_t _ready) {
    ready = _ready;
}

bool Channel::getInEpoll() {
    return inEpoll;
}

void Channel::setInEpoll() {
    inEpoll = true;
}
