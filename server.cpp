#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>

#include "Epoll.h"
#include "util.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"

#define BUF_SIZE 1024
#define MAX_EVENTS 1024

void handleReadEvent(int);

int main (int argc, char *argv[])
{
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    Epoll *ep = new Epoll();
    serv_sock->setnonblocking();
    //ep->addFd(serv_sock->getFd(), EPOLLIN | EPOLLET);
    Channel *servChannel = new Channel(ep, serv_sock->getFd());
    servChannel->enableReading();
    while (true) {
        std::vector<Channel*> activeChannels = ep->poll();
        for (int i = 0; i < activeChannels.size(); ++i) {
            if (activeChannels[i]->getFd() == serv_sock->getFd()) {
                InetAddress *clnt_addr = new InetAddress();
                Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
                printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
                clnt_sock->setnonblocking();
                //ep->addFd(clnt_sock->getFd(), EPOLLIN | EPOLLET);
                Channel *clntChannel = new Channel(ep, clnt_sock->getFd());
                clntChannel->enableReading();
            } else if (activeChannels[i]->getRevents() & EPOLLIN)
                handleReadEvent(activeChannels[i]->getFd());
            else
                printf("something else happened\n");
        }
    }
    delete serv_sock;
    delete serv_addr;
    return 0;
}

void handleReadEvent(int sockfd) {
    char buf[BUF_SIZE];
    while (true) {
        bzero(buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, bytes_read);
        } else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            printf("finish reading once, errno: %d\n", errno);
            break;
        } else if (bytes_read == -1 && errno == EINTR) {
            printf("continue reading\n");
            continue;
        } else if (bytes_read == 0) {
            printf("EOF, client fd %d disconnected\n", sockfd);
            close(sockfd);
            break;
        }
    }
}
