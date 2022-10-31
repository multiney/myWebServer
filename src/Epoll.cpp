#include "./include/Epoll.h"
#include "./include/Channel.h"
#include "./include/Util.h"

#include <cstddef>
#include <cstring>
#include <strings.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

#define MAX_EVENTS 1024

Epoll::Epoll() {
    epfd_ = epoll_create1(0);
    errif(epfd_ == -1, "epoll create error");
    events_ = new epoll_event[MAX_EVENTS];
    memset(events_, 0, sizeof(*events_) * MAX_EVENTS);
}

Epoll::~Epoll() {
    if (epfd_ != -1) {
        close(epfd_);
        epfd_ = -1;
    }
    delete [] events_;
}

void Epoll::updateChannel(Channel *channel) {
    int fd = channel->getFd();
    struct epoll_event event {};
    event.data.ptr = channel;
    event.events = channel->getEvents();
    if (channel->getInEpoll()) {
        errif(epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &event) == -1, "epoll modify error");
    } else {
        errif(epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &event) == -1, "epoll add error");
        channel->setInEpoll(true);
    }
}

std::vector<Channel*> Epoll::poll(int timeout) {
    int nfds = epoll_wait(epfd_, events_, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    std::vector<Channel*> activeChannels;
    for (int i = 0; i < nfds; ++i) {
        Channel *ch = (Channel*) events_[i].data.ptr;
        ch->setReady(events_[i].events);// TODO: events[i].events == channel->getEvents() ????
        activeChannels.push_back(ch);
    }
    return activeChannels;
}

void Epoll::deleteChannel(Channel *channel) {
    int fd = channel->getFd();
    errif(epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr) == -1, "epoll delete error");
    channel->setInEpoll(false);
}
