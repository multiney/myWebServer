#pragma once

#include <functional>

#include "Macros.h"

class EventLoop;
class Channel;
class Socket;
class Buffer;
class Connection {
 public:
    enum State {
        Invalid = 1,
        Handshaking,
        Connected,
        Closed,
        Failed,
    };
    Connection(EventLoop *_loop, Socket *_sock);
    ~Connection();
    DISALLOW_COPY_AND_MOVE(Connection);

    void Read();
    void Write();

    void SetDelConnectionCallback(std::function<void(int)> const &callback);
    void SetOnConnectionCallback(std::function<void(Connection *)> const &callback);
    State GetState();
    void Close();
    void SetSendBuffer(const char *str);
    Buffer *GetReadBuffer();
    const char* ReadBuffer();
    Buffer* GetSendBuffer();
    const char* SendBuffer();
    void GetLineSendBuffer();
    Socket* GetSocket();

    //void OnConnect(std::function<void()> fn);

 private:
    EventLoop *loop_;
    Socket *sock_;
    Channel *channel_;
    State state_{State::Invalid};
    Buffer *read_buffer_;
    Buffer *send_buffer_;
    std::function<void(int)> del_connection_callback_;
    std::function<void(Connection *)> on_connect_callback_;

    void ReadNonBlocking();
    void WriteNonBlocking();
    void ReadBlocking();
    void WriteBlocking();
};
