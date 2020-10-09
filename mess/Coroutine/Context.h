#pragma once
#include <functional>
#define USE_ASM_CONTEXT 
#ifdef USE_THREAD_CONTEXT
#include <thread>
#include <mutex>
#elif !defined(USE_ASM_CONTEXT)
#define USE_UCONTEXT 1
#include <ucontext.h>
using coroutine_context_t = ucontext_t;
#else
extern "C"{
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

typedef void *fcontext_t;
using coroutine_context_t = fcontext_t;
intptr_t jump_fcontext(fcontext_t * ofc, fcontext_t nfc,
        intptr_t vp, bool preserve_fpu = false);

fcontext_t make_fcontext(void* sp, size_t size, void (*fn)(intptr_t));
}
#endif

using coroutine_func_t = std::function<void(void *)>;

class Context
{
public:
        Context(size_t stack_size, const coroutine_func_t &fn, void *private_data);
        ~Context();
        bool swapIn();
        bool swapOut();
        inline bool isEnd(){return _end;}
        static void context_func(void *arg);
protected:
        coroutine_func_t _fn;
#ifdef USE_THREAD_CONTEXT
        std::thread _thread;
        std::mutex _lock;
        std::mutex *_swap_lock;
#else
        coroutine_context_t _ctx;
        coroutine_context_t _swap_ctx;
        char *_stack;
        uint32_t _stack_size;
#endif
        void *_private_data;
        bool _end;
};