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
public:
    TimerQueue(EventLoop *loop);
    ~TimerQueue();
    void addTimer(int32_t intervalMs, TimerCallBack cb, bool loop = false);
    void ReadCallBack();
    void addTimerInLoop(Timer *timer);
    std::vector<std::pair<int64_t, Timer *>> getExpired(int64_t iNow);
    void resetTimer(int64_t iExpireMs);
    void reset(const std::vector<std::pair<int64_t, Timer *>> &vExpired, int64_t iNow);

private:
    bool insertTimer(Timer *timer);
    using TimerSet = std::set<std::pair<int64_t, Timer *>>;
    EventLoop *m_pLoop;
    int m_iFd;
    std::unique_ptr<Channel> m_chan;
    TimerSet m_setTimers;
};
#endif