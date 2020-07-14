#include "EventLoop.h"
#include "EventLoopThread.h"

EventLoopThread::EventLoopThread()
{
    _thread = std::thread(std::bind(&EventLoopThread::threadFunc, this));
}

EventLoopThread::~EventLoopThread()
{
    if(_loop)
    {
        _loop->quit();
        _thread.join();
    }
}

EventLoop *EventLoopThread::start()
{
    EventLoop *loop;
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _cond.wait(lock, [this](){return this->_loop != nullptr;});
        loop = _loop;
    }

    return loop;
}

void EventLoopThread::threadFunc()
{
    EventLoop loop;
    {
        std::unique_lock<std::mutex> lock(_mtx);
        _loop = &loop;
        _cond.notify_all();
    }
    loop.loop();
    std::lock_guard<std::mutex> lock(_mtx);
    _loop = nullptr;
}
