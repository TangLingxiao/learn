#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

class EventLoop;

class EventLoopThread
{
public:
    EventLoopThread();
    ~EventLoopThread();

    EventLoop* start();
private:
    void threadFunc();
private:
    std::thread _thread;
    std::mutex _mtx;
    std::condition_variable _cond;
    EventLoop *_loop{nullptr};
};