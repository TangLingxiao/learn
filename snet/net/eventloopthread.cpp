#include "eventloopthread.h"
#include "eventloop.h"
#include <thread>

EventLoopThread::EventLoopThread() : m_mtx(), m_cond(), m_pLoop(nullptr), m_thread(std::thread(&EventLoopThread::threadFunc, this))
{
}

EventLoopThread::~EventLoopThread()
{
    if (m_pLoop != nullptr)
    {
        m_pLoop->quit();
    }
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

EventLoop *EventLoopThread::startEventLoop()
{
    EventLoop *loop = nullptr;
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        while (m_pLoop == nullptr)
        {
            m_cond.wait(lock);
        }
        loop = m_pLoop;
    }
    return loop;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_pLoop = &loop;
        m_cond.notify_one();
    }
    loop.loop();
    std::lock_guard<std::mutex> lock(m_mtx);
    m_pLoop = nullptr;
}