#include "Epoll.h"
#include "Util.h"

#include <sys/epoll.h>
#include <unistd.h>

#define MAXEVENTS 1024

Epoll::Epoll() : epfd(-1), events(nullptr) {
    epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");
    events = new epoll_event[MAXEVENTS];
}

Epoll::~Epoll() {
    if (epfd != -1) {
        close(epfd);
        epfd = -1;
    }
    delete [] events;
}
