#include "src/EventLoop.h"
#include "src/Server.h"

int main (int argc, char *argv[])
{
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    loop->loop();
    delete server;
    delete loop;
    return 0;
}
