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
    static int64_t getNow()
    {
        return static_cast<int64_t>(time(0));
    }
    static int64_t getNowMs()
    {
        timeval time;
        if (gettimeofday(&time, nullptr) < 0)
        {
            return 0;
        }
        return static_cast<int64_t>(time.tv_sec * kMicroSecondsPerSecond + time.tv_usec);
    }
    static int64_t getTimeDiff(int64_t time1, int64_t time2)
    {
        if (time1 < time2)
        {
            return 0;
        }
        return time1 - time2;
    }
    static std::string formatTime(int64_t time)
    {
        char buf[32];
        time_t timet = time;
        tm result;
        localtime_r(&timet, &result);
        snprintf(buf, sizeof buf, "%4d-%02d-%02d %02d:%02d:%02d",
                 result.tm_year + 1900, result.tm_mon + 1, result.tm_mday, result.tm_hour, result.tm_min, result.tm_sec);
        return buf;
    }
    static const int64_t kMicroSecondsPerSecond = 1000 * 1000;
};

class Timer : public NonCopyable
{
public:
    explicit Timer(double iSecond, TimerCallBack cb, bool bloop = false)
        : m_bLoop(bloop), m_iExpire(TimeUtil::getNowMs() + static_cast<int64_t>(TimeUtil::kMicroSecondsPerSecond * iSecond)), m_interval(iSecond), m_cb(std::move(cb)) {}
    ~Timer() {}
    void onTimer()
    {
        m_cb();
    }
    void reStart(int64_t iNow)
    {
        if (m_bLoop)
        {
            m_iExpire = iNow + static_cast<int64_t>(m_interval * TimeUtil::kMicroSecondsPerSecond);
        }
    }
    int64_t getExpire() const
    {
        return m_iExpire;
    }

    bool loop()
    {
        return m_bLoop;
    }

private:
    bool m_bLoop;
    int64_t m_iExpire;
    double m_interval;
    TimerCallBack m_cb;
};

#endif
