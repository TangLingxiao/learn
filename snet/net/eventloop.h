#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "base/noncopyable.h"
#include <thread>
#include <vector>
#include <memory>
#include "base/callback.h"
#include <mutex>

class Channel;
class Poller;
class TimerQueue;
class EventLoop : public NonCopyable
{
public:
    using Functor = std::function<void()>;
    EventLoop();
    ~EventLoop();

    void loop();
    static EventLoop *getEventLoopOfCurrentThread();
    bool inLoopThread();
    void quit();
    void updateChannel(Channel *channel);
    void runAfter(double iSeconds, TimerCallBack cb, bool loop = false);
    void runInLoop(Functor cb);

private:
    using PendingFuncs = std::vector<Functor>;
    void queueInLoop(Functor cb);
    void handleReadWakeChannel();
    void wakeupWakeChannel();
    void runAllPending();
private:
    using ChannelList = std::vector<Channel *>;
    bool m_bLooping = false;
    std::thread::id m_tId;
    bool m_bQuit = true;
    std::unique_ptr<Poller> m_poller;
    std::unique_ptr<TimerQueue> m_pTimerQueue;
    ChannelList m_vActiveChannels;
    int32_t m_iWakeupFd;
    std::unique_ptr<Channel> m_cWakeupChannel;
    PendingFuncs m_vPendingFuncs;
    std::mutex m_mtx;
    bool m_bRunPendingFuncs;
};
#endif