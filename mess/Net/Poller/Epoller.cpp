#include "Epoller.h"
#include "EventLoop.h"
#include "Channel.h"
#include <sys/epoll.h>
#include <cstring>
#include "Log/Logger.h"
#include <cerrno>

std::string getEpollDes(int32_t opt)
{
    switch (opt)
    {
    case EPOLL_CTL_ADD:
        return "EPOLL_CTL_ADD";
    case EPOLL_CTL_MOD:
        return "EPOLL_CTL_MOD";
    case EPOLL_CTL_DEL:
        return "EPOLL_CTL_DEL";
    }
    return "";
}

Epoller::Epoller(EventLoop *loop) : Poller(loop), _epollFds(128)
{
    _fd = ::epoll_create1(EPOLL_CLOEXEC);
}

void Epoller::updateChannel(Channel *channel)
{
    const int32_t index = channel->index();
    if (index == E_NEW || index == E_DELETED)
    {
        if (index == E_NEW)
        {
            _channels[channel->fd()] = channel;
        }
        channel->setIndex(E_ADDED);
        update(EPOLL_CTL_ADD, channel);
    }
    else
    {
        if (channel->isNoneEvents())
        {
            channel->setIndex(E_DELETED);
            update(EPOLL_CTL_DEL, channel);
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void Epoller::removeChannel(Channel *channel)
{
    if (_channels.find(channel->fd()) != _channels.end())
    {
        if (channel->index() == E_ADDED)
        {
            update(EPOLL_CTL_DEL, channel);
        }
        _channels.erase(channel->fd());
    }
}

void Epoller::fillActiveChannels(int32_t events, ChannelList *activeChannels) const
{
    for (int32_t i = 0; i < events; ++i)
    {
        auto *channel = static_cast<Channel *>(_epollFds[i].data.ptr);
        channel->setRevents(_epollFds[i].events);
        activeChannels->push_back(channel);
    }
}

size_t Epoller::poll(int64_t timeout, ChannelList *activeChannels)
{
    int32_t nums = ::epoll_wait(_fd, &*_epollFds.begin(), _epollFds.size(), timeout);
    if (nums > 0)
    {
        fillActiveChannels(nums, activeChannels);
        if (nums == static_cast<int32_t>(_epollFds.size()))
        {
            _epollFds.resize(2 * _epollFds.size());
        }
        LOG_DEBUG(nums << " events happened");
    }
    else if (nums == 0)
    {
        LOG_DEBUG("nothing happened");
    }
    else
    {
        LOG_ERROR("epoll error");
    }
    return nums;
}

void Epoller::update(int32_t opt, Channel *channel)
{
    epoll_event evt;
    memset(&evt, 0, sizeof evt);
    evt.events = channel->events();
    evt.data.ptr = channel;
    int32_t ret = ::epoll_ctl(_fd, opt, channel->fd(), &evt);
    if (ret != 0)
    {
        LOG_ERROR("epoll_ctl failed, opt:" << getEpollDes(opt) << ", errno:" << errno);
    }
    else
    {
        LOG_DEBUG("epoll_ctl success, opt:" << getEpollDes(opt));
    }
}