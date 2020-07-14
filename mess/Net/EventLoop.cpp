#include "Channel.h"
#include "Poller.h"
#include "Poller/Epoller.h"
#include "EventLoop.h"
#include "Timer/TimerMgr.h"
EventLoop::EventLoop() : _threadId(std::this_thread::get_id()), _poller(new Epoller(this))
{
    TimerMgr::getInstance().init();
}

EventLoop::~EventLoop()
{
    TimerMgr::getInstance().destroy();
}

bool EventLoop::inLoopThread()
{
    return _threadId == std::this_thread::get_id();
}

void EventLoop::quit()
{
    _terminate = true;
}

void EventLoop::loop()
{
    while (!_terminate)
    {
        TimerMgr::getInstance().update(0);
        ChannelList activeChannels;
        _poller->poll(10, &activeChannels);
        for (auto channel : activeChannels)
        {
            channel->handleEventWithGuard();
        }
        _pending.SweepOnce(std::bind(&EventLoop::runPending, this, std::placeholders::_1));
    }
}

void EventLoop::updateChannel(Channel *channel)
{
    _poller->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
    _poller->removeChannel(channel);
}

void EventLoop::runInLoop(VoidCb cb)
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

void EventLoop::queueInLoop(VoidCb cb)
{
    PendingCb *pcb = new PendingCb;
    pcb->cb = std::move(cb);
    _pending.InsertHead(pcb);
}

void EventLoop::runPending(PendingCb *cb)
{
    cb->cb();
    delete cb;
}

void EventLoop::runAfter(int64_t delayMs, VoidCb cb)
{
    TimerMgr::getInstance().setTimer(delayMs, 1, cb);
}