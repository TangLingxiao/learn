#ifndef __TIMERQUEUE_H__
#define __TIMERQUEUE_H__
#include "noncopyable.h"
#include <set>
#include <memory>
#include "callback.h"
#include <vector>

class Timer;
class EventLoop;
class Channel;

class TimerQueue : public NonCopyable
{
    using Entry = std::pair<int64_t, Timer *>;
public:
    TimerQueue(EventLoop *loop);
    ~TimerQueue();
    void addTimer(double iSeconds, TimerCallBack cb, bool loop = false);
    void handleRead();
    void addTimerInLoop(Timer *timer);
private:
    std::vector<Entry> getExpired(int64_t iNow);
    void resetTimer(int64_t iExpireMs);
    bool insertTimer(Timer *timer);
    void reset(const std::vector<Entry> &vExpired, int64_t iNow);
private:
    using TimerSet = std::set<Entry>;
    EventLoop *m_pLoop;
    int m_iFd;
    std::unique_ptr<Channel> m_chan;
    TimerSet m_setTimers;
};
#endif