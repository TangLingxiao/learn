#pragma once
#include "Pattern/Singleton.h"
#include <vector>
#include <queue>
#include <stdint.h>
#include <functional>

class TimerBase;
class TimerWheel;

class TimerMgr : public Singleton<TimerMgr>
{
public:
    int32_t init();
    void destroy();
    void update(int64_t now);

    int64_t getLeftTime(uint64_t timer_id);
    TimerBase* findTimer(uint64_t timer_id);
    void killTimer(uint64_t timer_id);

    uint64_t setTimer(int64_t interval, uint32_t count, std::function<void()>);
    void recycleTimer(TimerBase *timer);
private:
    TimerBase* getTimerBase();
private:
    std::vector<TimerBase*> _timers;
    std::queue<TimerBase*> _timer_pool;
    TimerWheel* _timer_wheel;
};