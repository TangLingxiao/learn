#include "AsyncQueue.h"
#include "AsyncThread.h"

bool AsyncQueue::ready()
{
    return true;
}

bool AsyncQueue::init(int32_t threadNum)
{
    for(int32_t i = 0; i < threadNum; ++i)
    {
        AsyncThread *t = new AsyncThread(_queueId, i);
        t->start();
        _threads.push_back(t);
    }
    return true;
}

bool AsyncQueue::destroy()
{
    for(auto t : _threads)
    {
        t->terminate();
        delete t;
    }
    _threads.clear();
    delete this;
    return true;
}

void AsyncQueue::loop()
{
    for(auto t : _threads)
    {
        t->loop();
    }
}

void AsyncQueue::startAsync(const int64_t threadid, IAsyncHandler *handler, const char *file, const int32_t line)
{
    AsyncBase *base = new AsyncBase(handler, file, line);
    _threads[static_cast<size_t>(threadid) % _threads.size()]->add(base);
}

void AsyncQueue::stopAsync(IAsyncHandler *handler)
{
    static_cast<AsyncBase*>(handler->getTrace())->setInvalid();
}

int32_t AsyncQueue::getQueueId()
{
    return _queueId;
}

void AsyncQueue::getQueueInfo(int32_t &threadNum, std::set<int32_t> &threadIds)
{
    threadNum = static_cast<int32_t>(_threads.size());
    for(size_t i = 0; i < _threads.size(); ++i)
    {
        threadIds.insert(i);
    }
}
