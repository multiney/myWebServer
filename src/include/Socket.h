#pragma once

#include <arpa/inet.h>

class InetAddress {
public:
    InetAddress();
    InetAddress(const char *ip, uint16_t port);
    ~InetAddress() = default;

    void SetAddr(sockaddr_in addr);
    sockaddr_in GetAddr();
    const char *GetIp();
    uint16_t GetPort();
private:
    struct sockaddr_in addr_ {};
};

class Socket
{
private:
    int fd_{-1};
public:
    Socket();
    explicit Socket(int sockfd);
    ~Socket();

    void Bind(InetAddress *addr);
    void Listen();
    int Accept(InetAddress *addr);
    void Connect(InetAddress *addr);
    void Setnonblocking();

    int GetFd();
};
