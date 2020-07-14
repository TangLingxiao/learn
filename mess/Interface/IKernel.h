#pragma once
#include <stdint.h>
#include <set>
class IKernel;

class ITrace
{
public:
    virtual ~ITrace() = default;
    virtual const char *getTraceInfo() = 0;
};

class IAsyncHandler
{
public:
    IAsyncHandler() = default;
    virtual ~IAsyncHandler() = default;

    inline void setTrace(ITrace *trace) { _trace = trace; }
    inline ITrace *getTrace() { return _trace; }

    virtual bool onExecute(IKernel *kernel, int32_t queueId, int32_t threadid) = 0;
    virtual bool onSuccess(IKernel *kernel) = 0;
    virtual bool onFailed(IKernel *kernel) = 0;
    virtual bool onRelease(IKernel *kernel) = 0;

private:
    ITrace *_trace{nullptr};
};

class IAsyncQueue
{
public:
    virtual ~IAsyncQueue() = default;

    virtual void startAsync(const int64_t threadid, IAsyncHandler *handler, const char *file, const int32_t line) = 0;
    virtual void stopAsync(IAsyncHandler *handler) = 0;
    virtual int32_t getQueueId() = 0;
    virtual void getQueueInfo(int32_t &threadNum, std::set<int32_t> &threadIds) = 0;
};

class IKernel
{
public:
    virtual ~IKernel() = default;
    //log
    virtual void syncLog(const char *) = 0;
    virtual void asyncLog(const char *) = 0;

    //net

    //timer
    virtual void killTimer(uint64_t) = 0;
    virtual uint64_t setTimer(void* args, int64_t interval, uint32_t count, void(*cb)(void*)) = 0;

    //async
    virtual void startAsync(const int64_t threadid, IAsyncHandler *handler, const char* file, const int32_t line) = 0;
    virtual void stopAsync(IAsyncHandler *handler) = 0;
    virtual IAsyncQueue *getMainAsyncQueue() = 0;
    virtual IAsyncQueue *createAsyncQueue(const int32_t threadSize, const char *trace) = 0;
};