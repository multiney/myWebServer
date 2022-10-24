#include "Acceptor.h"
#include "EventLoop.h"
#include "Socket.h"
#include "InetAddress.h"

Acceptor::Acceptor(EventLoop *_loop) : loop(_loop), sock(nullptr), addr(nullptr) {
    sock = new Socket();
    addr = new InetAddress("127.0.0.1", 1234);

}
