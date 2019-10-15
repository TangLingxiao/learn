#ifndef __EPOLLPOLLER_H__
#define __EPOLLPOLLER_H__

#include "net/poller.h"

struct epoll_event;
class EPollPoller : public Poller
{
public:
    EPollPoller(EventLoop *loop);
    ~EPollPoller();

    void updateChannel(Channel *channel) override;
    void removeChannel(Channel *channel) override;
    uint32_t poll(int32_t timeout, ChannelList *activeChannels) override;

private:
    void update(int32_t iOp, Channel *channel);
    void fillActiveChannels(int32_t iEvents, ChannelList *activeChannels) const override;
    using EpollFdList = std::vector<epoll_event>;
    EpollFdList m_vEpollfds;
    int32_t m_iFd;
};
#endif