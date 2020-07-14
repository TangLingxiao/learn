#include "EventLoopThread.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "Pool/ObjectPool.h"
#include <functional>
EventLoopThreadPool::EventLoopThreadPool(EventLoop *l):_loop(l)
{
}

EventLoopThreadPool::~EventLoopThreadPool()
{
}

void EventLoopThreadPool::setThreadNum(int32_t num)
{
    _threadNum = num;
    INIT_OBJECT_POOL(EventLoopThread, _threadNum);
}
void EventLoopThreadPool::start()
{
    for(int32_t i = 0; i < _threadNum; ++i)
    {
        EventLoopThread *p = NEW_OBJECT(EventLoopThread);
        _pool.emplace_back(std::unique_ptr<EventLoopThread, std::function<void(EventLoopThread*)>>(p, [](EventLoopThread* raw){DELETE_OBJECT(raw);}));
        _loops.emplace_back(p->start());
    }
}
EventLoop *EventLoopThreadPool::getLoop()
{
    EventLoop* loop = _loop;
    if(_loops.size() > 0)
    {
        loop = _loops[_next];
        if(++_next >= static_cast<int32_t>(_loops.size()))
        {
            _next = 0;
        }
    }
    return loop;
}