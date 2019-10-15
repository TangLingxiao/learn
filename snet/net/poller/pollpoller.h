#ifndef __POLLPOLLER_H__
#define __POLLPOLLER_H__

#include "net/poller.h"

struct pollfd;
class PollPoller : public Poller
{
public:
    PollPoller(EventLoop *loop);
    ~PollPoller();

    void updateChannel(Channel *channel) override;
    void removeChannel(Channel *channel) override;
    uint32_t poll(int32_t timeout, ChannelList *activeChannels) override;

private:
    void fillActiveChannels(int32_t iEvents, ChannelList *activeChannels) const override;
    using PollFdList = std::vector<pollfd>;
    PollFdList m_vPollfds;
};
#endif