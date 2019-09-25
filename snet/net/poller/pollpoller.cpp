#include "pollpoller.h"
#include <poll.h>
#include "net/eventloop.h"
#include "net/channel.h"
#include "base/logmgr.h"
#include <cassert>

PollPoller::PollPoller(EventLoop *loop)
    : Poller(loop)
{
}

PollPoller::~PollPoller()
{
}

uint32_t PollPoller::poll(int32_t timeout, ChannelList *activeChannels)
{
    int32_t iNums = ::poll(&*m_vPollfds.begin(), m_vPollfds.size(), timeout);
    int32_t iNow = static_cast<uint32_t>(time(0));
    if (iNums > 0)
    {
        LOG_INFO(iNums << " events happened");
        fillActiveChannels(iNums, activeChannels);
    }
    else if (iNums == 0)
    {
        LOG_INFO("nothing happened");
    }
    else
    {
        LOG_ERROR("PollPoller::poll error");
    }
    return iNow;
}
void PollPoller::updateChannel(Channel *channel)
{
    assert(channel != nullptr);
    assert(Poller::inLoopThread());
    LOG_INFO("fd = " << channel->fd() << " events = " << channel->events());
    if (channel->index() < 0)
    {
        //add new channel to poll
        assert(m_mChannels.find(channel->fd()) == m_mChannels.end());
        m_mChannels[channel->fd()] = channel;
        channel->setIndex(static_cast<int32_t>(m_vPollfds.size()));
        struct pollfd fd;
        fd.fd = channel->fd();
        fd.events = static_cast<short>(channel->events());
        fd.revents = 0;
        m_vPollfds.push_back(fd);
    }
    else
    {
        //update existing one
        assert(m_mChannels.find(channel->fd()) != m_mChannels.end());
        assert(m_mChannels[channel->fd()] == channel);
        int32_t idx = channel->index();
        assert(idx >= 0 && idx < static_cast<int32_t>(m_vPollfds.size()));
        auto &fd = m_vPollfds[idx];
        assert(fd.fd == channel->fd() || fd.fd == -1);

        fd.events = static_cast<short>(channel->events());
        fd.revents = 0;
        if (channel->isNoneEvent())
        {
            fd.fd = -1;
        }
    }
}
void PollPoller::fillActiveChannels(int32_t iEvents, ChannelList *activeChannels) const
{
    for (auto it = m_vPollfds.begin(); it != m_vPollfds.end() && iEvents > 0; ++it)
    {
        if (it->revents > 0)
        {
            --iEvents;
            auto ch = m_mChannels.find(it->fd);
            assert(ch != m_mChannels.end());
            auto channel = ch->second;
            assert(channel->fd() == it->fd);
            channel->setREvents(it->revents);
            activeChannels->push_back(channel);
        }
    }
}