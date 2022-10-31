#include "./include/Channel.h"
#include "./include/EventLoop.h"

#include <sys/epoll.h>
#include <unistd.h>

Channel::Channel(EventLoop *_loop, int _fd)
    : loop(_loop), fd(_fd), events(0), ready(0), inEpoll(false) {

}

Channel::~Channel() {
}

void Channel::setReadCallback(std::function<void ()> const &callback) {
    readCallback = callback;
}

void Channel::enableRead() {
    events |= EPOLLIN | EPOLLPRI;
    loop->updateChannel(this);
}

void Channel::useET() {
    events |= EPOLLET;
    loop->updateChannel(this);
}

void Channel::handleEvent() {
    if (ready & (EPOLLIN | EPOLLPRI)) {
        readCallback();
    }
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

void Channel::setInEpoll(bool _in) {
    inEpoll = _in;
}
