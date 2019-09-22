#include "eventloop.h"
#include "base/logmgr.h"
#include "channel.h"
#include "poller.h"
#include <cassert>
#include "base/timerqueue.h"
#include <sys/eventfd.h>
#include <unistd.h>

thread_local EventLoop *t_loopInThisThread = nullptr;

EventLoop::EventLoop()
    : m_tId(std::this_thread::get_id()), m_poller(new Poller(this)), m_pTimerQueue(new TimerQueue(this)), m_iWakeupFd(::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)), m_cWakeupChannel(new Channel(this, m_iWakeupFd)),m_bRunPendingFuncs(false)
{
    LOG_INFO("EventLoop created " << this << " in thread " << m_tId);
    if (t_loopInThisThread)
    {
        LOG_ERROR("Another EventLoop " << t_loopInThisThread << " exists in this thread" << m_tId);
    }
    else
    {
        t_loopInThisThread = this;
    }
    m_cWakeupChannel->setReadCallback(std::bind(&EventLoop::handleReadWakeChannel, this));
    m_cWakeupChannel->enableReading();
}

EventLoop::~EventLoop()
{
    assert(!m_bLooping);
    t_loopInThisThread = nullptr;
    ::close(m_iWakeupFd);
}

EventLoop *EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

bool EventLoop::inLoopThread()
{
    return m_tId == std::this_thread::get_id();
}

void EventLoop::loop()
{
    assert(!m_bLooping);
    assert(inLoopThread());
    m_bLooping = true;
    m_bQuit = false;
    while (!m_bQuit)
    {
        m_vActiveChannels.clear();
        m_poller->poll(10000, &m_vActiveChannels);
        for (auto channel : m_vActiveChannels)
        {
            channel->handleEvent();
        }
        runAllPending();
    }
    LOG_INFO("EventLoop " << this << " stop looping");
    m_bLooping = false;
}
void EventLoop::quit()
{
    m_bQuit = true;
    //非io线程quit时,需要wakeup
    if(!inLoopThread())
    {
        wakeupWakeChannel();
    }
}
void EventLoop::updateChannel(Channel *channel)
{
    assert(channel != nullptr);
    assert(channel->getEventLoop() == this);
    assert(inLoopThread());
    m_poller->updateChannel(channel);
}

void EventLoop::runAfter(double iSeconds, TimerCallBack cb, bool loop)
{
    m_pTimerQueue->addTimer(iSeconds, std::move(cb), loop);
}

void EventLoop::runInLoop(Functor cb)
{
    if (inLoopThread())
    {
        cb();
    }
    else
    {
        queueInLoop(std::move(cb));
    }
}

void EventLoop::queueInLoop(Functor cb)
{
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_vPendingFuncs.emplace_back(std::move(cb));
    }
    
    //m_bRunPendingFuncs当runAllPending中调用queueInloop时需要wakeup
    //handleevent调用queueinloop不需要wakeup，因为之后会调用runallpending
    if (!inLoopThread() || m_bRunPendingFuncs)
    {
        wakeupWakeChannel();
    }
}
void EventLoop::handleReadWakeChannel()
{
    uint64_t iNum = 0;
    auto size = ::read(m_iWakeupFd, &iNum, sizeof iNum);
    if (size != sizeof iNum)
    {
        LOG_ERROR("eventloop handleread wakechannel error");
    }
}
void EventLoop::wakeupWakeChannel()
{
    uint64_t iNum = 1;
    auto size = ::write(m_iWakeupFd, &iNum, sizeof iNum);
    if (size != sizeof iNum)
    {
        LOG_ERROR("eventloop wakeupchannel error");
    }
}
void EventLoop::runAllPending()
{
    PendingFuncs vPendingFuncs;
    m_bRunPendingFuncs = true;
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        std::swap(m_vPendingFuncs, vPendingFuncs);
    }
    for (auto func : vPendingFuncs)
    {
        func();
    }
    m_bRunPendingFuncs = false;
}