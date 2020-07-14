#pragma once
#include <vector>
#include <memory>

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool
{
public:
    EventLoopThreadPool(EventLoop *loop);
    ~EventLoopThreadPool();

    void setThreadNum(int32_t num);
    void start();
    EventLoop *getLoop();

private:
    EventLoop *_loop{nullptr};
    int32_t _threadNum{0};
    int32_t _next{0};
    std::vector<std::unique_ptr<EventLoopThread, std::function<void(EventLoopThread*)>>> _pool;
    std::vector<EventLoop *> _loops;
};