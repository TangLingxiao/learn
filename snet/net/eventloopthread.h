#ifndef __EVENTLOOPTHREAD_H__
#define __EVENTLOOPTHREAD_H__

#include <mutex>
#include <thread>
#include <condition_variable>
#include "base/noncopyable.h"

class EventLoop;

class EventLoopThread : public NonCopyable
{
public:
    EventLoopThread();
    ~EventLoopThread();
    EventLoop *getEventLoop();

private:
    void threadFunc();

private:
    std::mutex m_mtx;
    std::condition_variable m_cond;
    EventLoop *m_pLoop;
    std::thread *m_thread;
};
#endif