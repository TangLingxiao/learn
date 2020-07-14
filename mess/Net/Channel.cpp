#include "Channel.h"
#include <poll.h>
#include "EventLoop.h"
#include "Log/Logger.h"

const int32_t Channel::_noneEvent = 0;
const int32_t Channel::_readEvent = POLLIN | POLLPRI;
const int32_t Channel::_writeEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int32_t fd1) : _loop(loop), _fd(fd1)
{
}

void Channel::update()
{
    _loop->updateChannel(this);
}

void Channel::remove()
{
    _loop->removeChannel(this);
}

void Channel::handleEventWithGuard()
{
    if (_tied)
    {
        auto lock = _tie.lock();
        if (lock)
        {
            handleEvent();
        }
    }
    else
    {
        handleEvent();
    }
}

void Channel::handleEvent()
{
    if ((_revents & POLLHUP) && !(_revents & POLLIN))
    {
        if (_closeCb)
        {
            _closeCb();
        }
    }
    if (_revents & POLLNVAL)
    {
        LOG_WARN("Channel::handleEvent POLLNVAL");
    }
    if (_revents & (POLLERR | POLLNVAL))
    {
        if (_errorCb)
        {
            _errorCb();
        }
    }
    if (_revents & (_readEvent | POLLRDHUP))
    {
        if (_readCb)
        {
            _readCb();
        }
    }
    if (_revents & _writeEvent)
    {
        if (_writeCb)
        {
            _writeCb();
        }
    }
}

void Channel::tie(const std::shared_ptr<void> &ptr)
{
    _tied = true;
    _tie = ptr;
}