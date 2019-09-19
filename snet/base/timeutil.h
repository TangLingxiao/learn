#ifndef __TIMEUTIL_H__
#define __TIMEUTIL_H__

#include <string>
#include <sys/time.h>
#include <time.h>
#include "noncopyable.h"
#include "callback.h"

class TimeUtil
{
public:
    static uint32_t getNow();
    static uint32_t getNowMs();
    static int32_t getTimeDiff(uint32_t time1, uint32_t time2);
    static std::string formatTime(uint32_t time);
};

uint32_t TimeUtil::getNow()
{
    return time(0);
}

uint32_t TimeUtil::getNowMs()
{
    timeval time;
    if (gettimeofday(&time, nullptr) < 0)
    {
        return 0;
    }
    return time.tv_sec * 1000 + time.tv_usec / 1000;
}

int32_t TimeUtil::getTimeDiff(uint32_t time1, uint32_t time2)
{
    if (time1 < time2)
    {
        return 0;
    }
    return time1 - time2;
}

std::string TimeUtil::formatTime(uint32_t time)
{
    char buf[32];
    time_t timet = time;
    tm result;
    gmtime_r(&timet, &result);
    snprintf(buf, sizeof buf, "%4d%02d%02d %02d:%02d:%02d",
             result.tm_year + 1900, result.tm_mon + 1, result.tm_mday, result.tm_hour, result.tm_min, result.tm_sec);
    return buf;
}

class Timer : public NonCopyable
{
public:
    explicit Timer(int32_t intervalMs, TimerCallBack cb, bool loop = false)
        : m_bLoop(loop), m_iExpire(TimeUtil::getNowMs() + intervalMs), m_cb(std::move(cb)), m_intervalMs(intervalMs) {}
    ~Timer() {}
    void onTimer()
    {
        m_cb();
    }
    void reStart()
    {
        if (m_bLoop)
        {
            m_iExpire += m_intervalMs;
        }
    }
    uint32_t getExpire() const
    {
        return m_iExpire;
    }

private:
    bool m_bLoop;
    uint32_t m_iExpire;
    int32_t m_intervalMs;
    TimerCallBack m_cb;
};

#endif
