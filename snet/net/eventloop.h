#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "base/noncopyable.h"
#include <thread>
#include <vector>

class Channel;
class Poller;
class EventLoop : public NonCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    static EventLoop *getEventLoopOfCurrentThread();
    bool checkInLoopThread();
    void quit();
    void updateChannel(Channel *channel);

private:
    using ChannelList = std::vector<Channel *>;

    bool m_bLooping = false;
    std::thread::id m_tId;
    bool m_bQuit = true;
    Poller *m_poller = nullptr;
    ChannelList m_vActiveChannels;
};
#endif