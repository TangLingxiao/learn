#pragma once
#include "Structure/RingBuffer.h"
#include <memory>
#include <sys/socket.h>
#include <map>
class Socket;
class Channel;
class EventLoop;
class Coroutine;
class CoSocket
{
public:
    CoSocket(EventLoop *lp, int32_t fd);
    ~CoSocket();
    void handleWrite();
    void handleRead();
    ssize_t coSend(const void *buf, size_t len, int flags);
    ssize_t coRead(void *buf, size_t count);
    ssize_t coWrite(const void *buf, size_t len);
    ssize_t coRecv(void *buf, size_t len, int flags);
    int coConnect(const struct sockaddr *addr, socklen_t addrlen);
    ssize_t coSendmsg(const struct msghdr *msg, int flags);
    ssize_t coRecvmsg(struct msghdr *msg, int flags);

private:
    EventLoop *_loop;
    std::unique_ptr<Socket> _socket;
    std::unique_ptr<Channel> _channel;
    Coroutine *_read_co;
    Coroutine *_write_co;
    RingBuff _recvBuf;
    RingBuff _sendBuf;
};

static std::map<int32_t, std::unique_ptr<CoSocket>> s_mSockets;