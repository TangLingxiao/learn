#include "timerqueue.h"
#include "timeutil.h"
#include "logmgr.h"
#include <sys/timerfd.h>
#include "net/channel.h"
#include "net/eventloop.h"
#include <memory.h>

TimerQueue::TimerQueue(EventLoop *loop)
    : m_pLoop(loop), m_iFd(::timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC)), m_chan(new Channel(loop, m_iFd))
{
    m_chan->setReadCallback(std::bind(&TimerQueue::ReadCallBack, this));
    m_chan->enableReading();
}
TimerQueue::~TimerQueue()
{
}
void TimerQueue::addTimer(int32_t intervalMs, TimerCallBack cb, bool loop = false)
{
    std::unique_ptr<Timer> timer(new Timer(intervalMs, std::move(cb), loop));
}

void TimerQueue::ReadCallBack()
{
}

void TimerQueue::addTimerCallBack(std::unique_ptr<Timer> timer)
{
}
void TimerQueue::resetTimer(uint32_t iExpireMs)
{
    itimerspec oNewTimer;
    itimerspec oOldTimer;
    memset(&oNewTimer, 0, sizeof oNewTimer);
    memset(&oOldTimer, 0, sizeof oOldTimer);
    oNewTimer.it_interval.tv_sec = iExpireMs / 1000;
    oNewTimer.it_interval.tv_nsec = iExpireMs % 1000000 * 1000;
    ::timerfd_settime(m_iFd, 0, &oNewTimer, &oOldTimer);
}