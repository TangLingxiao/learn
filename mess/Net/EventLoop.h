#pragma once
#include <memory>
#include <vector>
#include <mutex>
#include <thread>
#include "CallBack.h"
#include "Pattern/NonCopyable.h"
#include "Structure/LockFreeList.h"

class Poller;
class Channel;

struct PendingCb
{
    VoidCb cb;
    AtomicIntrusiveLinkedListHook<PendingCb> next;
};

class EventLoop : public NonCopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool inLoopThread();
    void quit();
    void runInLoop(VoidCb cb);   //可以把一些操作移到io线程
    void queueInLoop(VoidCb cb); //可以把一些操作移到io线程

    void runAfter(int64_t delayMs, VoidCb cb);
private:
    void runPending(PendingCb *cb);

private:
    std::thread::id _threadId;
    std::mutex _mtx;
    std::unique_ptr<Poller> _poller;
    bool _terminate{false};
    AtomicIntrusiveLinkedList<PendingCb, &PendingCb::next> _pending;
};