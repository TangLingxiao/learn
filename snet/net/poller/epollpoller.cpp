#include "epollpoller.h"
#include <sys/epoll.h>
#include "net/eventloop.h"
#include "net/channel.h"
#include "base/logmgr.h"
#include <cassert>
#include <unistd.h>
#include <memory.h>
#include <string>

std::string getEpollDes(int32_t iOp);

EPollPoller::EPollPoller(EventLoop *loop)
    : Poller(loop), m_vEpollfds(128), m_iFd(::epoll_create1(EPOLL_CLOEXEC))
{
}

EPollPoller::~EPollPoller()
{
    ::close(m_iFd);
}

uint32_t EPollPoller::poll(int32_t timeout, ChannelList *activeChannels)
{
    int32_t iNums = ::epoll_wait(m_iFd, &*m_vEpollfds.begin(), static_cast<int32_t>(m_vEpollfds.size()), timeout);
    int32_t iNow = static_cast<uint32_t>(time(0));
    if (iNums > 0)
    {
        if (static_cast<size_t>(iNums) == m_vEpollfds.size())
        {
            m_vEpollfds.resize(2 * m_vEpollfds.size());
        }
        LOG_INFO(iNums << " events happened");
        fillActiveChannels(iNums, activeChannels);
    }
    else if (iNums == 0)
    {
        LOG_INFO("nothing happened");
    }
    else
    {
        LOG_ERROR("EPollPoller::epoll error");
    }
    return iNow;
}
void EPollPoller::updateChannel(Channel *channel)
{
    assert(channel != nullptr);
    assert(Poller::inLoopThread());
    const int32_t index = channel->index();
    const int32_t fd = channel->fd();
    if (index == Channel::E_NEW || index == Channel::E_DELETED)
    {
        if (index == Channel::E_NEW)
        {
            assert(m_mChannels.find(fd) == m_mChannels.end());
            m_mChannels[fd] = channel;
        }
        else
        {
            assert(m_mChannels.find(fd) != m_mChannels.end());
            assert(m_mChannels[fd] == channel);
        }
        update(EPOLL_CTL_ADD, channel);
        channel->setIndex(Channel::E_ADDED);
    }
    else
    {
        //mod or del
        assert(m_mChannels.find(fd) != m_mChannels.end());
        assert(m_mChannels[fd] == channel);

        if (channel->isNoneEvent())
        {
            update(EPOLL_CTL_DEL, channel);
            channel->setIndex(Channel::E_DELETED);
        }
        else
        {
            update(EPOLL_CTL_MOD, channel);
        }
    }
}

void EPollPoller::update(int32_t iOp, Channel *channel)
{
    epoll_event oEvt;
    memset(&oEvt, 0, sizeof oEvt);
    oEvt.events = channel->events();
    oEvt.data.ptr = channel;
    int32_t iRet = ::epoll_ctl(m_iFd, iOp, channel->fd(), &oEvt);
    if (iRet != 0)
    {
        LOG_ERROR("epoll ctl error");
    }
    else
    {
        LOG_INFO("epoll ctl, op: " << getEpollDes(iOp));
    }
}
void EPollPoller::fillActiveChannels(int32_t iEvents, ChannelList *activeChannels) const
{
    for (int32_t i = 0; i < iEvents; ++i)
    {
        auto channel = static_cast<Channel *>(m_vEpollfds[i].data.ptr);
        assert(channel != nullptr);
        auto it = m_mChannels.find(channel->fd());
        assert(it != m_mChannels.end());
        assert(it->second == channel);
        channel->setREvents(m_vEpollfds[i].events);
        activeChannels->push_back(channel);
    }
}

std::string getEpollDes(int32_t iOp)
{
    switch (iOp)
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

void EPollPoller::removeChannel(Channel *channel)
{
    assert(channel != nullptr);
    assert(Poller::inLoopThread());
    const int32_t index = channel->index();
    const int32_t fd = channel->fd();
    assert(m_mChannels.find(fd) != m_mChannels.end());
    assert(m_mChannels[fd] == channel);
    assert(channel->isNoneEvent());
    if (index == Channel::E_ADDED)
    {
        update(EPOLL_CTL_DEL, channel);
    }
    m_mChannels.erase(fd);
    channel->setIndex(Channel::E_NEW);
}