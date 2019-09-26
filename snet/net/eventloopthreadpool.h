#ifndef __EVENTLOOPTHREADPOOL_H__
#define __EVENTLOOPTHREADPOOL_H__

#include <vector>
#include <memory>
#include "base/noncopyable.h"

class EventLoop;
class EventLoopThread;
class EventLoopThreadPool : public NonCopyable
{
public:
    EventLoopThreadPool(int32_t iThreadNum, EventLoop *loop);
    ~EventLoopThreadPool();
    void start();
    EventLoop *getNextLoop();

private:
    int32_t m_iThreadNum;
    EventLoop *m_pLoop;
    int32_t m_iNext;
    std::vector<EventLoop *> m_vLoops;
    std::vector<std::unique_ptr<EventLoopThread>> m_vLoopThreads;
    bool m_bStarted;
};
#endif