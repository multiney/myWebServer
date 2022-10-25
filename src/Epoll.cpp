#include "Epoll.h"
#include "Channel.h"
#include "Util.h"

#include <strings.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>

#define MAXEVENTS 1024

Epoll::Epoll() : epfd(-1), events(nullptr) {
    epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");
    events = new epoll_event[MAXEVENTS];
    bzero(events, sizeof(*events) * MAXEVENTS);
}

Epoll::~Epoll() {
    if (epfd != -1) {
        close(epfd);
        epfd = -1;
    }
    delete [] events;
}

void Epoll::updateChannel(Channel *channel) {
    int fd = channel->getFd();
    struct epoll_event event;
    bzero(&event, sizeof(event));
    event.data.ptr = channel;
    event.events = channel->getEvents();
    if (channel->getInEpoll()) {
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event) == -1, "epoll modify error");
    } else {
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) == -1, "epoll add error");
        channel->setInEpoll();
    }
}

std::vector<Channel*> Epoll::poll(int timeout) {
    int nfds = epoll_wait(epfd, events, MAXEVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    std::vector<Channel*> activeChannels;
    for (int i = 0; i < nfds; ++i) {
        Channel *ch = (Channel*) events[i].data.ptr;
        ch->setReady(events[i].events);// TODO: events[i].events == channel->getEvents() ????
        activeChannels.push_back(ch);
    }
    return activeChannels;
}
