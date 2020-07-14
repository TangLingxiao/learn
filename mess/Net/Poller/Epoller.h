#pragma once
#include "Poller.h"

struct epoll_event;
class Epoller : public Poller
{
    enum{E_NEW = -1, E_ADDED, E_DELETED};
public:
    Epoller(EventLoop *loop);
    virtual void updateChannel(Channel *channel) override;
    virtual void removeChannel(Channel *channel) override;
    virtual size_t poll(int64_t timeout, ChannelList *activeChannels) override;

protected:
    virtual void fillActiveChannels(int32_t events, ChannelList *activeChannels) const override;

private:
    void update(int32_t opt, Channel *channel);

private:
    int32_t _fd{-1};
    std::vector<epoll_event> _epollFds;
};