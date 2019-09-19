#ifndef __TIMERQUEUE_H__
#define __TIMERQUEUE_H__
#include "noncopyable.h"
#include <set>
#include <memory>
#include "callback.h"

class Timer;
class EventLoop;
class Channel;

class TimerQueue : public NonCopyable
{
public:
    TimerQueue(EventLoop *loop);
    ~TimerQueue();
    void addTimer(int32_t intervalMs, TimerCallBack cb, bool loop = false);
    void ReadCallBack();
    void addTimerCallBack(std::unique_ptr<Timer> timer);
    void resetTimer(uint32_t iExpireMs);
private:
    using TimerSet = std::set<uint32_t, std::unique_ptr<Timer>>;
    EventLoop *m_pLoop;
    int m_iFd;
    std::unique_ptr<Channel> m_chan;
    TimerSet m_setTimers;
};
#endif