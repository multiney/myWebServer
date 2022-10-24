#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *_loop, int _fd) : loop(_loop), fd(_fd), events(0), ready(0), inEpoll(false) {

}

Channel::~Channel() {

}
