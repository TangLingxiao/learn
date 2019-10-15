#include <cassert>
#include "timerqueue.h"
#include "timeutil.h"
#include "logmgr.h"
#include <sys/timerfd.h>
#include "net/channel.h"
#include "net/eventloop.h"
#include <memory.h>
#include <sys/unistd.h>
#include <cassert>

TimerQueue::TimerQueue(EventLoop *loop)
    : m_pLoop(loop), m_iFd(::timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC)), m_chan(new Channel(loop, m_iFd))
{
    m_chan->setReadCallback(std::bind(&TimerQueue::handleRead, this));
    m_chan->enableReading();
}
TimerQueue::~TimerQueue()
{
    ::close(m_iFd);
    for (auto &timer : m_setTimers)
    {
        delete timer.second;
    }
}
void TimerQueue::addTimer(double iSeconds, TimerCallBack cb, bool loop)
{
    Timer *timer = new Timer(iSeconds, std::move(cb), loop);
    m_pLoop->runInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));
}

void TimerQueue::handleRead()
{
    assert(m_pLoop->inLoopThread());
    uint64_t iNum = 0;
    auto nLen = ::read(m_iFd, &iNum, sizeof iNum);
    if (nLen != sizeof iNum)
    {
        LOG_ERROR("handleRead error");
    }
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
    assert(m_pLoop->inLoopThread());
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
    m_setTimers.emplace(std::make_pair(timer->getExpire(), timer));
    return bReset;
}

void TimerQueue::reset(const std::vector<Entry> &vExpired, int64_t iNow)
{
    for (const auto &timer : vExpired)
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

struct timespec howMuchTimeFromNow(int64_t iExpireMs)
{
    int64_t iMs = iExpireMs - TimeUtil::getNowMs();
    if (iMs < 100)
    {
        iMs = 100;
    }
    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(
        iMs / TimeUtil::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(
        (iMs % TimeUtil::kMicroSecondsPerSecond) * 1000);
    return ts;
}

void TimerQueue::resetTimer(int64_t iExpireMs)
{
    itimerspec oNewTimer;
    itimerspec oOldTimer;
    memset(&oNewTimer, 0, sizeof oNewTimer);
    memset(&oOldTimer, 0, sizeof oOldTimer);
    oNewTimer.it_value = howMuchTimeFromNow(iExpireMs);
    int ret = ::timerfd_settime(m_iFd, 0, &oNewTimer, &oOldTimer);
    if (ret)
    {
        LOG_ERROR("timerfd_settime error");
    }
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(int64_t iNow)
{
    std::vector<Entry> vExpired;
    auto tmp = std::make_pair(iNow, reinterpret_cast<Timer *>(UINTPTR_MAX));
    auto end = std::lower_bound(m_setTimers.begin(), m_setTimers.end(), tmp);
    assert(end == m_setTimers.end() || iNow < end->first);
    std::copy(m_setTimers.begin(), end, std::back_inserter(vExpired));
    m_setTimers.erase(m_setTimers.begin(), end);
    return vExpired;
}