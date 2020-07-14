#pragma once

#include <map>
#include <vector>
#include "Pattern/NonCopyable.h"

class Channel;
class EventLoop;
using ChannelList = std::vector<Channel*>;
class Poller : public NonCopyable
{
public:
    using ChannelMap = std::map<int32_t, Channel*>;
    Poller(EventLoop *loop):_loop(loop){}
    virtual ~Poller(){}

    virtual void updateChannel(Channel* channel) = 0;
    virtual void removeChannel(Channel* channel) = 0;
    virtual size_t poll(int64_t timeout, ChannelList *activeChannels) = 0;
    
protected:
    virtual void fillActiveChannels(int32_t events, ChannelList *activeChannels) const = 0;
    ChannelMap _channels;
private:
    EventLoop * _loop;
};