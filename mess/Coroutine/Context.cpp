#include "Context.h"
#include "Log/Logger.h"

Context::Context(size_t stack_size, const coroutine_func_t &fn, void *private_data)
    : _fn(fn), _stack_size(stack_size), _private_data(private_data)
{
    _end = false;
    _stack = (char *)malloc(_stack_size);
    if (!_stack)
    {
        LOG_ERROR("failed to malloc stack memory.");
        exit(254);
    }
    LOG_DEBUG("alloc stack: size=" << _stack_size << ", ptr=" << _stack);

#if USE_UCONTEXT
    if (-1 == getcontext(&_ctx))
    {
        free(_stack);
        throw std::runtime_error("getcontext failed");
        return;
    }
    _ctx.uc_stack.ss_sp = _stack;
    _ctx.uc_stack.ss_size = _stack_size;
    _ctx.uc_link = nullptr;
    makecontext(&_ctx, (void (*)(void)) & context_func, 1, this);
#else
    void *sp = (void *)((char *)_stack + _stack_size);
    _ctx = make_fcontext(sp, _stack_size, (void (*)(intptr_t)) & context_func);
    _swap_ctx = nullptr;
#endif
}

Context::~Context()
{
    if (_stack)
    {
        free(_stack);
        _stack = nullptr;
    }
}

bool Context::swapIn()
{
#if USE_UCONTEXT
    return 0 == swapcontext(&_swap_ctx, &_ctx);
#else
    jump_fcontext(&_swap_ctx, _ctx, (intptr_t)this, true);
    return true;
#endif
}

bool Context::swapOut()
{
#if USE_UCONTEXT
    return 0 == swapcontext(&_ctx, &_swap_ctx);
#else
    jump_fcontext(&_ctx, _swap_ctx, (intptr_t)this, true);
    return true;
#endif
}

void Context::context_func(void *arg)
{
    Context *_this = (Context *)arg;
    _this->_fn(_this->_private_data);
    _this->_end = true;
    _this->swapOut();
}