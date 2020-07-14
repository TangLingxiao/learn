#pragma once
#include "../Interface/IKernel.h"
#include "../Structure/List.h"
#include <thread>
#include <string>
#include <mutex>

class AsyncBase : public ITrace, public ILinkNode
{
public:
    AsyncBase(IAsyncHandler*handler, const char *file, int32_t line):_handler(handler)
    {
        _handler->setTrace(this);
    }

    virtual ~AsyncBase() = default;
    const char* getTraceInfo() override {return _debug.c_str();}
    inline void setInvalid(){_valid = false;}

    void onExecute(int32_t queueId, int32_t threadIdx)
    {
        if(_valid)
        {
            _successed = _handler->onExecute(nullptr, queueId, threadIdx);
            _executed = true;
        }
    }

    void onComplete()
    {
        if(_successed)
        {
            _handler->onSuccess(nullptr);
        }
        else
        {
            _handler->onFailed(nullptr);
        }
        _handler->onRelease(nullptr);
    }

    void release()
    {
        delete this;
    }
public:
    AsyncBase *next{nullptr};
private:
    std::string _debug;
    IAsyncHandler * _handler{nullptr};
    bool _valid{true};
    bool _executed{false};
    bool _successed{false};
};

class AsyncThread
{
    struct AsyncBaseThreadData
    {
        LinkList main;
        LinkList swap;
        LinkList work;
        std::mutex lock;
        AsyncBaseThreadData()
        {
            main.clear();
            swap.clear();
            work.clear();
        }
    };

public:
    AsyncThread(int32_t queueId, int32_t threadIdx);
    ~AsyncThread() = default;

    void start();
    void terminate();
    void add(AsyncBase *base);
    void loop();

private:
	void threadProc();
private:
    AsyncThread(const AsyncThread&) = delete;
private:
    bool _terminate{false};
    int32_t _queueId{0};
    int32_t _threadIdx{0};
    std::thread _thread;
    AsyncBaseThreadData _complete;
    AsyncBaseThreadData _ready;
};