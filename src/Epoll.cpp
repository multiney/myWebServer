#include "./include/Epoll.h"

#include <sys/epoll.h>
#include <unistd.h>

#include <vector>
#include <cstring>

#include "./include/Channel.h"
#include "./include/Util.h"


#define MAX_EVENTS 1024

Epoll::Epoll()
    : epfd_(epoll_create1(0)) {
    ErrorIf(epfd_ == -1, "epoll create error");
    events_ = new epoll_event[MAX_EVENTS];
    std::memset(events_, 0, sizeof(*events_) * MAX_EVENTS);
}

Epoll::~Epoll() {
    if (epfd_ != -1) {
        close(epfd_);
        epfd_ = -1;
    }
    delete [] events_;
}

void Epoll::UpdateChannel(Channel *channel) {
    int sockfd = channel->GetFd();
    struct epoll_event event {};
    event.data.ptr = channel;
    event.events = channel->GetEvents();
    if (channel->GetInEpoll()) {
        ErrorIf(epoll_ctl(epfd_, EPOLL_CTL_MOD, sockfd, &event) == -1, "epoll modify error");
    } else {
        ErrorIf(epoll_ctl(epfd_, EPOLL_CTL_ADD, sockfd, &event) == -1, "epoll add error");
        channel->SetInEpoll(true);
    }
}

std::vector<Channel*> Epoll::Poll(int timeout) {
    int nfds = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);
    ErrorIf(nfds == -1, "epoll wait error");
    std::vector<Channel*> active_channels;
    for (int i = 0; i < nfds; ++i) {
        Channel *channel = (Channel*) events_[i].data.ptr;
        channel->SetReady(events_[i].events);// TODO: events[i].events == channel->getEvents() ????
        active_channels.push_back(channel);
    }
    return active_channels;
}

void Epoll::DeleteChannel(Channel *channel) {
    int sockfd = channel->GetFd();
    ErrorIf(epoll_ctl(epfd_, EPOLL_CTL_DEL, sockfd, nullptr) == -1, "epoll delete error");
    channel->SetInEpoll(false);
}
