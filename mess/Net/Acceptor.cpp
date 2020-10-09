#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Log/Logger.h"
#include <unistd.h>

Acceptor::Acceptor(EventLoop *loop, const std::string &ip, uint16_t port)
    : _loop(loop), _sock(new Socket(createSocketfd())), _channel(new Channel(loop, _sock->fd()))
{
    _sock->setReuse(true);
    _sock->setTcpNoDelay(true);
    _sock->bind(InetAddr(ip, port));
    _channel->setReadCb(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
}

void Acceptor::setNewConnectionCb(NewConnectionCb callback)
{
    _cb = std::move(callback);
}

void Acceptor::listen()
{
    _sock->listen();
    _channel->enableReading();
}

void Acceptor::handleRead()
{
    InetAddr addr;
    int32_t fd = _sock->accept(&addr);
    if (fd > 0)
    {
        if (_cb)
        {
            _cb(fd, addr);
            LOG_TRACE("new connection, " << addr.toString());
        }
        else
        {
            ::close(fd);
        }
    }
    else
    {
        LOG_ERROR("acceptor handleread error");
    }
}
