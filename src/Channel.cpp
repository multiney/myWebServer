#include "./include/Channel.h"

#include <sys/epoll.h>

#include "./include/EventLoop.h"

Channel::Channel(EventLoop *_loop, int sockfd)
    : loop_(_loop), fd_(sockfd), events_(0), ready_(0), in_epoll_(false) {

}

void Channel::SetReadCallback(std::function<void ()> const &callback) {
    read_callback_ = callback;
}

void Channel::EnableRead() {
    events_ |= EPOLLIN | EPOLLPRI;
    loop_->UpdateChannel(this);
}

void Channel::UseET() {
    events_ |= EPOLLET;
    loop_->UpdateChannel(this);
}

void Channel::HandleEvent() {
    if (ready_ & (EPOLLIN | EPOLLPRI)) {
        read_callback_();
    }
}

int Channel::GetFd() {
    return fd_;
}

uint32_t Channel::GetEvents() {
    return events_;
}

uint32_t Channel::GetReady() {
    return ready_;
}

void Channel::SetReady(uint32_t ready) {
    ready_ = ready;
}

bool Channel::GetInEpoll() {
    return in_epoll_;
}

void Channel::SetInEpoll(bool inEpoll) {
    in_epoll_ = inEpoll;
}
