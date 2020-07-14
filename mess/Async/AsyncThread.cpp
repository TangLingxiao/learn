#include "AsyncThread.h"
#include <unistd.h>
AsyncThread::AsyncThread(int32_t queueId, int32_t threadIdx) : _queueId(queueId), _threadIdx(threadIdx)
{
}

void AsyncThread::start()
{
    _thread = std::thread(&AsyncThread::threadProc, this);
}

void AsyncThread::terminate()
{
    _terminate = true;
    _thread.join();
}

void AsyncThread::add(AsyncBase *base)
{
    _ready.main.push_back(base);
}

void AsyncThread::loop()
{
    mergeListByLock(_ready.swap, _ready.main, _ready.lock);
    mergeListByLock(_complete.main, _complete.swap, _complete.lock);
    auto base = static_cast<AsyncBase*>(_complete.main.pop_front());
    while(base)
    {
        base->onComplete();
        base->release();
        base = static_cast<AsyncBase*>(_complete.main.pop_front());
    }

}

void AsyncThread::threadProc()
{
    while(!_terminate)
    {
        auto base = static_cast<AsyncBase*>(_ready.work.pop_front());
        if(!base)
        {
            mergeListByLock(_ready.work, _ready.swap, _ready.lock);
            base = static_cast<AsyncBase*>(_ready.work.pop_front());
        }
        if(base)
        {
            base->onExecute(_queueId, _threadIdx);
            _complete.work.push_back(base);
            if(_complete.swap.empty())
            {
                mergeListByLock(_complete.swap, _complete.work, _complete.lock);
            }
        }
        else
        {
            usleep(1000* 1000);
        }
    }
}