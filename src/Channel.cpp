#include "Channel.h"
#include "EventLoop.h"

#include <sys/epoll.h>
#include <unistd.h>

Channel::Channel(EventLoop *_loop, int _fd)
    : loop(_loop), fd(_fd), events(0), ready(0), inEpoll(false), useThreadPool(true) {

}

Channel::~Channel() {
}

void Channel::setReadCallback(std::function<void ()> _cb) {
    readCallback = _cb;
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
        if (useThreadPool)
            loop->addThread(readCallback);
        else
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

void Channel::setUseThreadPool(bool use) {
    useThreadPool = use;
}
