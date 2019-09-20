#include <cassert>
#include "timerqueue.h"
#include "timeutil.h"
#include "logmgr.h"
#include <sys/timerfd.h>
#include "net/channel.h"
#include "net/eventloop.h"
#include <memory.h>
#include <sys/unistd.h>

TimerQueue::TimerQueue(EventLoop *loop)
    : m_pLoop(loop), m_iFd(::timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC)), m_chan(new Channel(loop, m_iFd))
{
    m_chan->setReadCallback(std::bind(&TimerQueue::ReadCallBack, this));
    m_chan->enableReading();
}
TimerQueue::~TimerQueue()
{
}
void TimerQueue::addTimer(int32_t intervalMs, TimerCallBack cb, bool loop)
{
    Timer *timer = new Timer(intervalMs, std::move(cb), loop);
    addTimerInLoop(timer);
}

void TimerQueue::ReadCallBack()
{
    uint64_t iNum;
    auto nLen = ::read(m_iFd, &iNum, sizeof iNum);
    assert(nLen == sizeof iNum);
    auto iNow = TimeUtil::getNowMs();
    auto vExpired = getExpired(iNow);
    for (const auto &oExpired : vExpired)
    {
        oExpired.second->onTimer();
    }
    reset(vExpired, iNow);
}

void TimerQueue::addTimerInLoop(Timer *timer)
{
    bool bReset = insertTimer(timer);
    if (bReset)
    {
        resetTimer(m_setTimers.begin()->first);
    }
}

bool TimerQueue::insertTimer(Timer *timer)
{
    assert(timer != nullptr);
    bool bReset = m_setTimers.empty() || m_setTimers.begin()->first > timer->getExpire();
    m_setTimers.emplace(std::move(std::make_pair(timer->getExpire(), std::move(timer))));
    return bReset;
}

void TimerQueue::reset(const std::vector<std::pair<int64_t, Timer *>> &vExpired, int64_t iNow)
{
    for (auto timer : vExpired)
    {
        assert(timer.second != nullptr);
        if (timer.second->loop())
        {
            timer.second->reStart(iNow);
            insertTimer(timer.second);
        }
        else
        {
            delete timer.second;
            timer.second = nullptr;
        }
    }
    int64_t iNextExpire = 0;
    if (!m_setTimers.empty())
    {
        iNextExpire = m_setTimers.begin()->first;
    }
    if (iNextExpire > 0)
    {
        resetTimer(iNextExpire);
    }
}

void TimerQueue::resetTimer(int64_t iExpireMs)
{
    itimerspec oNewTimer;
    itimerspec oOldTimer;
    memset(&oNewTimer, 0, sizeof oNewTimer);
    memset(&oOldTimer, 0, sizeof oOldTimer);
    oNewTimer.it_value.tv_sec = iExpireMs / 1000;
    oNewTimer.it_value.tv_nsec = iExpireMs % 1000000 * 1000;
    int ret = ::timerfd_settime(m_iFd, TFD_TIMER_ABSTIME, &oNewTimer, &oOldTimer);
    if(ret)
    {
        LOG_ERROR("timerfd_settime erroe");
    }
}

std::vector<std::pair<int64_t, Timer *>> TimerQueue::getExpired(int64_t iNow)
{
    std::vector<std::pair<int64_t, Timer *>> vExpired;
    auto tmp = std::make_pair(iNow, reinterpret_cast<Timer *>(UINTPTR_MAX));
    auto end = std::lower_bound(m_setTimers.begin(), m_setTimers.end(), tmp);
    assert(end == m_setTimers.end() || iNow < end->first);
    std::copy(m_setTimers.begin(), m_setTimers.end(), std::back_inserter(vExpired));
    m_setTimers.erase(m_setTimers.begin(), end);
    return vExpired;
}