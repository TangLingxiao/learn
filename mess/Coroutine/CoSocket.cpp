#include "Net/Socket.h"
#include "Net/EventLoop.h"
#include "Net/Channel.h"
#include "Coroutine.h"
#include "Timer/TimerMgr.h"
#include "CoSocket.h"
#include <unistd.h>

#define RECV_BUFSIZE (64 * 1024)
#define SEND_BUFSIZE (64 * 1024)
EventLoop *g_loop = nullptr;
CoSocket::CoSocket(EventLoop *lp, int32_t fd) : _loop(lp),
                                                _socket(new Socket(fd)),
                                                _channel(new Channel(_loop, fd)), _recvBuf(RECV_BUFSIZE), _sendBuf(SEND_BUFSIZE)
{
    _channel->setReadCb(std::bind(&CoSocket::handleRead, this));
    _channel->setWriteCb(std::bind(&CoSocket::handleWrite, this));
}
CoSocket::~CoSocket()
{
}

void CoSocket::handleWrite()
{
    _write_co->resume();
}

void CoSocket::handleRead()
{
    _read_co->resume();
}
ssize_t CoSocket::coSend(const void *buf, size_t len, int flags)
{
    Coroutine *co = Coroutine::getCurrentSafe();
    size_t remain = len;
    if (remain > 0)
    {
        size_t size = _sendBuf.TotalSize() - _sendBuf.Size();
        if (size < remain)
        {
            _sendBuf.Realloc(_sendBuf.Size() + remain);
        }
        size = _sendBuf.TotalSize() - _sendBuf.Size();
        LIB_ASSERT(size >= remain, "error sendbuf size");
        _sendBuf.Write(buf, remain);
        _channel->enableWriting();
        _write_co = co;
        _write_co->yield();
        _write_co = nullptr;
        _channel->disableWriting();
    }
    ssize_t wrote = ::send(_socket->fd(), buf, len, flags);
    _sendBuf.Clear();
    return wrote;
}
ssize_t CoSocket::coWrite(const void *buf, size_t len)
{
    Coroutine *co = Coroutine::getCurrentSafe();
    size_t remain = len;
    if (remain > 0)
    {
        size_t size = _sendBuf.TotalSize() - _sendBuf.Size();
        if (size < remain)
        {
            _sendBuf.Realloc(_sendBuf.Size() + remain);
        }
        size = _sendBuf.TotalSize() - _sendBuf.Size();
        LIB_ASSERT(size >= remain, "error sendbuf size");
        _sendBuf.Write(buf, remain);
        _channel->enableWriting();
        _write_co = co;
        _write_co->yield();
        _write_co = nullptr;
        _channel->disableWriting();
    }
    ssize_t wrote = ::write(_socket->fd(), buf, len);
    _sendBuf.Clear();
    return wrote;
}
ssize_t CoSocket::coRead(void *buf, size_t count)
{
    Coroutine *co = Coroutine::getCurrentSafe();
    _channel->enableReading();
    _read_co = co;
    _read_co->yield();
    _read_co = nullptr;
    return ::read(_socket->fd(), buf, count);
}
ssize_t CoSocket::coRecv(void *buf, size_t len, int flags)
{
    Coroutine *co = Coroutine::getCurrentSafe();
    _channel->enableReading();
    _read_co = co;
    _read_co->yield();
    _read_co = nullptr;
    return ::recv(_socket->fd(), buf, len, flags);
}

int CoSocket::coConnect(const struct sockaddr *addr, socklen_t addrlen)
{
    Coroutine *co = Coroutine::getCurrentSafe();
    int32_t fd = _socket->fd();
    int ret{0};
    do
    {
        ret = ::connect(fd, addr, addrlen);
    } while (ret < 0 && errno == EINTR);

    if (ret < 0)
    {
        if (errno != EINPROGRESS)
        {
            LOG_ERROR("connect error in CoSocket::coConnect " << errno);
        }
        else
        {
            _channel->enableWriting();
            _write_co = co;
            _write_co->yield();
            _write_co = nullptr;
            ret = 0;
        }
    }
    return ret;
}
ssize_t CoSocket::coSendmsg(const struct msghdr *msg, int flags)
{
    Coroutine *co = Coroutine::getCurrentSafe();
    _channel->enableWriting();
    _write_co = co;
    _write_co->yield();
    _write_co = nullptr;
    return ::sendmsg(_socket->fd(), msg, flags);
}

ssize_t CoSocket::coRecvmsg(struct msghdr *msg, int flags)
{
    Coroutine *co = Coroutine::getCurrentSafe();
    _channel->enableReading();
    _read_co = co;
    _read_co->yield();
    _read_co = nullptr;
    return ::recvmsg(_socket->fd(), msg, flags);
}