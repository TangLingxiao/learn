#include "eventloopthreadpool.h"
#include "eventloop.h"
#include "base/logmgr.h"
#include "eventloopthread.h"
#include <cassert>

EventLoopThreadPool::EventLoopThreadPool(int32_t iThreadNum, EventLoop *loop)
    : m_iThreadNum(iThreadNum), m_pLoop(loop), m_iNext(0), m_vLoops(), m_vLoopThreads(), m_bStarted(false)
{
}
EventLoopThreadPool::~EventLoopThreadPool()
{
}
void EventLoopThreadPool::start()
{
    assert(m_pLoop != nullptr);
    assert(m_pLoop->inLoopThread());
    if (m_bStarted)
    {
        return;
    }
    m_bStarted = true;
    for (int32_t i = 0; i < m_iThreadNum; ++i)
    {
        std::unique_ptr<EventLoopThread> oLoopThread(new EventLoopThread());
        EventLoop *loop = oLoopThread->getEventLoop();
        m_vLoops.push_back(loop);
        m_vLoopThreads.emplace_back(std::move(oLoopThread));
    }
}
EventLoop *EventLoopThreadPool::getNextLoop()
{
    assert(m_bStarted);
    assert(m_pLoop != nullptr);
    assert(m_pLoop->inLoopThread());
    assert(static_cast<size_t>(m_iNext) < m_vLoops.size());
    auto loop = m_vLoops[m_iNext];
    ++m_iNext;
    if (m_iNext >= m_iThreadNum)
    {
        m_iNext = 0;
    }
    return loop;
}