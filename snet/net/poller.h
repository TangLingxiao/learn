#ifndef __POLLER_H__
#define __POLLER_H__

#include <map>
#include <vector>
#include "base/noncopyable.h"

class Channel;
class EventLoop;
class Poller : public NonCopyable
{
public:
    using ChannelList = std::vector<Channel *>;
    Poller(EventLoop *loop) : m_mChannels(), m_pLoop(loop) {}
    virtual ~Poller() {}

    virtual void updateChannel(Channel *channel) = 0;
    virtual void removeChannel(Channel *channel) = 0;
    virtual uint32_t poll(int32_t timeout, ChannelList *activeChannels) = 0;
    bool inLoopThread();

protected:
    virtual void fillActiveChannels(int32_t iEvents, ChannelList *activeChannels) const = 0;
    using ChannelMap = std::map<int32_t, Channel *>;

    ChannelMap m_mChannels;

private:
    EventLoop *m_pLoop;
};
#endif