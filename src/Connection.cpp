#include "Connection.h"
#include "EventLoop.h"
#include "Channel.h"

Connection::Connection(EventLoop *_loop, int _fd) : loop(_loop), fd(_fd) {
    channel = new Channel(loop, fd);
}
