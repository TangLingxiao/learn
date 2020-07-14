#include "EventLoop.h"
#include "Connector.h"
#include "Channel.h"
#include "Socket.h"
#include "Log/Logger.h"
#include <cerrno>
#include <unistd.h>

#define MAX_RETRY_DELAYMS (30 * 1000)
#define INIT_RETRY_DELAYMS (500)

Connector::Connector(EventLoop *loop)
    : _loop(loop), _state(DISCONNECTED), _connect(false), _retryDelayMs(INIT_RETRY_DELAYMS)
{
}

Connector::~Connector()
{
}

void Connector::setNewConnectionCb(NewConnectionCb callback)
{
    _cb = std::move(callback);
}

void Connector::start(const std::string &ip, uint16_t port)
{
    _connect = true;
    _svraddr = InetAddr(ip, port);
    _loop->runInLoop(std::bind(&Connector::connect, this));
}

void Connector::restart()
{
    _state = DISCONNECTED;
    _connect = true;
    _retryDelayMs = INIT_RETRY_DELAYMS;
    connect();
}

void Connector::stop()
{
    _connect = false;
    _loop->queueInLoop(std::bind(&Connector::stopInLoop, this));
}

void Connector::stopInLoop()
{
    if(_state == CONNECTING)
    {
        _state = DISCONNECTED;
        int32_t fd = removeAndResetChannel();
        retry(fd);
    }
}

void Connector::connect()
{
    int32_t fd = createSocketfd();
    int32_t ret = net::connect(fd, _svraddr);
    int32_t saveError = (ret == 0) ? 0 : errno;
    switch (saveError)
    {
    case 0:
    case EINPROGRESS:
    case EINTR:
    case EISCONN:
        connecting(fd);
        break;

    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH:
        retry(fd);
        break;

    case EACCES:
    case EPERM:
    case EAFNOSUPPORT:
    case EALREADY:
    case EBADF:
    case EFAULT:
    case ENOTSOCK:
        LOG_ERROR("connect error in Connector::startInLoop " << saveError);
        ::close(fd);
        break;

    default:
        LOG_ERROR("Unexpected error in Connector::startInLoop " << saveError);
        ::close(fd);
        // connectErrorCallback_();
        break;
    }
}

void Connector::connecting(int32_t fd)
{
    _state = CONNECTING;
    _channel.reset(new Channel(_loop, fd));
    _channel->setWriteCb(std::bind(&Connector::handleWrite, this));
    _channel->setErrorCb(std::bind(&Connector::handleError, this));
    _channel->enableWriting();
}

void Connector::resetChannel()
{
    _channel.reset();
}

int32_t Connector::removeAndResetChannel()
{
    int32_t fd = _channel->fd();
    _channel->disableAll();
    _channel->remove();
    //连接之后connector channel就没用了，之后会用connection的channel
    //放到io线程，防止多线程冲突
    _loop->queueInLoop(std::bind(&Connector::resetChannel, this));
    return fd;
}

void Connector::handleWrite()
{
    if (_state == CONNECTING)
    {
        _state = CONNECTED;
        int32_t fd = removeAndResetChannel();
        if (_connect)
        {
            _cb(fd, _svraddr);
        }
        else
        {
            ::close(fd);
        }
    }
}

void Connector::handleError()
{
    LOG_ERROR("Connector::handleError, state:"<<_state);
    if(_state == CONNECTING)
    {
        int32_t fd = removeAndResetChannel();
        retry(fd);
    }
}

void Connector::retry(int32_t fd)
{
    ::close(fd);
    _state = DISCONNECTED;
    if(_connect)
    {
        _loop->runAfter(_retryDelayMs, std::bind(&Connector::connect, this));
        _retryDelayMs = std::min<int32_t>(_retryDelayMs * 2, MAX_RETRY_DELAYMS);
    }
}