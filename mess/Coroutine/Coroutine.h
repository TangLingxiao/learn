#pragma once
#include "Context.h"
#include <limits.h>
#include <string>
#include <unordered_map>
#include "Log/Logger.h"
#include <cassert>

#define DEFAULT_ALIGNMENT sizeof(unsigned long)
#define MEM_ALIGNED_SIZE(size) MEM_ALIGNED_SIZE_EX(size, DEFAULT_ALIGNMENT)
#define MEM_ALIGNED_SIZE_EX(size, alignment) (((size) + ((alignment)-1LL)) & ~((alignment)-1LL))
#ifndef MAX
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#endif
#ifndef MIN
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#endif
class Coroutine
{
public:
    void resume();
    void yield();

    void resume_naked();
    void yield_naked();

    constexpr static int STACK_ALIGNED_SIZE = (4 * 1024);
    constexpr static int MIN_STACK_SIZE = (64 * 1024);
    constexpr static int MAX_STACK_SIZE = (16 * 1024 * 1024);
    constexpr static long MAX_NUM_LIMIT = LONG_MAX;

    enum State
    {
        STATE_INIT = 0,
        STATE_WAITING,
        STATE_RUNNING,
        STATE_END,
    };

    enum Error
    {
        ERR_END = 0,
        ERR_LIMIT = -1,
        ERR_INVALID = -2,
    };

    using SwapCallback = std::function<void(void *)>;
    using BailoutCallback = std::function<void()>;

    inline enum State getState() { return _state; }
    inline long getCid() { return _cid; }
    inline Coroutine *getOrigin() { return _origin; }
    inline long getOriginCid() { return _origin ? _origin->getCid() : -1; }
    inline void *getTask() { return _task; }
    inline bool isEnd() { return _ctx.isEnd(); }
    inline void setTask(void *task) { _task = task; }
    static std::unordered_map<long, Coroutine *> _coroutines;
    static void setOnYield(SwapCallback func);
    static void setOnResume(SwapCallback func);
    static void setOnClose(SwapCallback func);
    static void bailout(BailoutCallback func);
    static inline long create(const coroutine_func_t &fn, void *args = nullptr)
    {
        return (new Coroutine(fn, args))->run();
    }
    static inline Coroutine *getCurrent() { return _current; }
    static inline Coroutine *getCurrentSafe()
    {
        if (!_current)
        {
            LOG_ERROR("");
        }
        return _current;
    }
    static inline void *getCurrentTask()
    {
        return _current ? _current->getTask() : nullptr;
    }
    static inline long getCurrentCid()
    {
        return _current ? _current->getCid() : -1;
    }
    static inline Coroutine *getByCid(long cid)
    {
        auto i = _coroutines.find(cid);
        return i != _coroutines.end() ? i->second : nullptr;
    }
    static inline void *getTaskByCid(long cid)
    {
        auto co = getByCid(cid);
        return co ? co->getTask() : nullptr;
    }
    static inline size_t getStackSize()
    {
        return _stack_size;
    }
    static inline void setStackSize(size_t size)
    {
        _stack_size = MEM_ALIGNED_SIZE_EX(MAX(MIN_STACK_SIZE, MIN(size, MAX_STACK_SIZE)), STACK_ALIGNED_SIZE);
    }
    static inline long getLastCid() { return _last_cid; }
    static inline size_t count() { return _coroutines.size(); }
    static inline uint64_t getPeakNum() { return _peak_num; }

protected:
    static Coroutine *_current;
    static long _last_cid;
    static uint64_t _peak_num;
    static size_t _stack_size;
    static SwapCallback _on_yield;
    static SwapCallback _on_resume;
    static SwapCallback _on_close;
    static BailoutCallback _on_bailout;

    enum State _state
    {
        STATE_INIT
    };
    long _cid;
    void *_task{nullptr};
    Context _ctx;
    Coroutine *_origin;

    Coroutine(const coroutine_func_t &fn, void *private_data) : _ctx(_stack_size, fn, private_data)
    {
        _cid = ++_last_cid;
        _coroutines[_cid] = this;
        if (count() > _peak_num)
        {
            _peak_num = count();
        }
    }

    inline long run()
    {
        long cid = this->_cid;
        _origin = _current;
        _current = this;
        _ctx.swapIn();
        checkEnd();
        return cid;
    }

    inline void checkEnd()
    {
        if (_ctx.isEnd())
        {
            close();
        }
        else if (_on_bailout)
        {
            assert(_current == nullptr);
            _on_bailout();
            exit(1);
        }
    }
    void close();
};

bool async(const std::function<void(void)> &fn, double timeout = -1);
bool run(const coroutine_func_t &fn, void *arg = nullptr);