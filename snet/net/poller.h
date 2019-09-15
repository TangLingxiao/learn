#ifndef __POLLER_H__
#define __POLLER_H__

#include <vector>
#include "base/noncopyable.h"
#include <stdint.h>
#include <map>

class Channel;
class EventLoop;
struct pollfd;
class Poller : public NonCopyable
{
public:
    using ChannelList = std::vector<Channel *>;
    Poller(EventLoop *loop);
    ~Poller();

    void updateChannel(Channel *channel);

    uint32_t poll(int32_t timeout, ChannelList *activeChannels);

private:
    void fillActiveChannels(int32_t iEvents, ChannelList *activeChannels) const;
    using PollFdList = std::vector<pollfd>;
    using ChannelMap = std::map<int32_t, Channel *>;
    EventLoop *m_pLoop;
    PollFdList m_vPollfds;
    ChannelMap m_mChannels;
};
#endif