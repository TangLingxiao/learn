#include "TimerMgr.h"
#include "Pool/ObjectPool.h"
#include "TimerWheel.h"

int32_t TimerMgr::init()
{
    _timer_wheel = NEW_OBJECT(TimerWheel);
    return 0;
}

void TimerMgr::destroy()
{
    for(auto timer : _timers)
    {
        DELETE_OBJECT(timer);
    }
    _timers.clear();
    DELETE_OBJECT(_timer_wheel);
}

void TimerMgr::update(int64_t now)
{
    _timer_wheel->runTimer();
}

int64_t TimerMgr::getLeftTime(uint64_t timer_id)
{
    auto *timer = findTimer(timer_id);
    if(timer)
    {
        int64_t _now_time = UtilTime::getNowMS();
        uint32_t left_count = timer->count > 1 ? timer->count - 1 : 0;
        int64_t left_time = timer->interval * left_count;
        left_time += timer->expires > _now_time ? timer->expires - _now_time : 0;
        return left_time;
    }
    return 0;
}

TimerBase *TimerMgr::findTimer(uint64_t timer_id)
{
    uint32_t index = (timer_id & std::numeric_limits<uint32_t>::max()) - 1;
    if(index >= _timers.size())
    {
        return nullptr;
    }
    auto *timer = _timers[index];
    if(timer && timer->timer_id == timer_id)
    {
        return timer;
    }
    LOG_ERROR("timer uid: " << timer_id << " find timer fail");
    return nullptr;
}

void TimerMgr::recycleTimer(TimerBase *timer)
{
    timer->callback = nullptr;
    _timer_pool.push(timer);
}

void TimerMgr::killTimer(uint64_t timer_id)
{
    auto *timer = findTimer(timer_id);
    if(!timer)
    {
        return;
    }
    timer->count = 0;
    if(timer->_prev)
    {
        timer->detach();
        recycleTimer(timer);
    }
}

TimerBase* TimerMgr::getTimerBase()
{
    TimerBase* timer{nullptr};
    if(!_timer_pool.empty())
    {
        timer = _timer_pool.front();
        _timer_pool.pop();
        timer->timer_id = (((timer->timer_id >> 32) + 1) << 32 | (timer->timer_id & std::numeric_limits<uint32_t>::max()));
    }
    else
    {
        timer = NEW_OBJECT(TimerBase);
        _timers.emplace_back(timer);
        timer->timer_id = 1ull << 32 | _timers.size();
    }
    return timer;
}

uint64_t TimerMgr::setTimer(int64_t interval, uint32_t count, std::function<void()> cb)
{
    if(interval <= 0)
    {
        cb();
        return 0;
    }
    auto *timer = getTimerBase();
    timer->count = count;
    timer->interval = interval;
    timer->callback = cb;
    timer->expires = UtilTime::getNowMS() + interval;

    _timer_wheel->addTimer(timer);
    return timer->timer_id;
}
