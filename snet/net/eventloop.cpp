#include "eventloop.h"
#include "base/logmgr.h"
#include "channel.h"
#include "poller.h"
#include <cassert>

thread_local EventLoop *t_loopInThisThread = nullptr;

EventLoop::EventLoop()
{
    m_tId = std::this_thread::get_id();
    LOG_INFO("EventLoop created " << this << " in thread " << m_tId);
    if (t_loopInThisThread)
    {
        LOG_ERROR("Another EventLoop " << t_loopInThisThread << " exists in this thread" << m_tId);
    }
    else
    {
        t_loopInThisThread = this;
    }
    m_poller = new Poller(this);
}

EventLoop::~EventLoop()
{
    assert(!m_bLooping);
    t_loopInThisThread = nullptr;
    delete m_poller;
    m_poller = nullptr;
}

EventLoop *EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

bool EventLoop::checkInLoopThread()
{
    return m_tId == std::this_thread::get_id();
}

void EventLoop::loop()
{
    assert(!m_bLooping);
    assert(checkInLoopThread());
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
    }
    LOG_INFO("EventLoop " << this << " stop looping");
    m_bLooping = false;
}
void EventLoop::quit()
{
    m_bQuit = true;
}
void EventLoop::updateChannel(Channel *channel)
{
    assert(channel != nullptr);
    assert(channel->getEventLoop() == this);
    assert(checkInLoopThread());
    m_poller->updateChannel(channel);
}