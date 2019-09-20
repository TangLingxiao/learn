#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "base/noncopyable.h"
#include <thread>
#include <vector>
#include <memory>
#include "base/callback.h"

class Channel;
class Poller;
class TimerQueue;
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
    void runAfter(int32_t iExpireMs, TimerCallBack cb);
private:
    using ChannelList = std::vector<Channel *>;

    bool m_bLooping = false;
    std::thread::id m_tId;
    bool m_bQuit = true;
    std::unique_ptr<Poller> m_poller;
    std::unique_ptr<TimerQueue> m_pTimerQueue;
    ChannelList m_vActiveChannels;
};
#endif