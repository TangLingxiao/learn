#include "Coroutine.h"
#include "HookApi.h"

#define DEFAULT_C_STACK_SIZE (2 * 1024 * 1024)
#define CORO_SUPPORT_BAILOUT 1
#define CORO_SWAP_BAILOUT 1

Coroutine *Coroutine::_current = nullptr;
long Coroutine::_last_cid = 0;
std::unordered_map<long, Coroutine *> Coroutine::_coroutines;
uint64_t Coroutine::_peak_num = 0;

size_t Coroutine::_stack_size = DEFAULT_C_STACK_SIZE;
Coroutine::SwapCallback Coroutine::_on_yield = nullptr;
Coroutine::SwapCallback Coroutine::_on_resume = nullptr;
Coroutine::SwapCallback Coroutine::_on_close = nullptr;
Coroutine::BailoutCallback Coroutine::_on_bailout = nullptr;

void Coroutine::yield()
{
    assert(_current == this || _on_bailout != nullptr);
    _state = STATE_WAITING;
    if(_on_yield)
    {
        _on_yield(_task);
    }
    _current = _origin;
    _ctx.swapOut();
}

void Coroutine::resume()
{
    assert(_current != this);
    if(_on_bailout)
    {
        return;
    }
    _state = STATE_RUNNING;
    if(_on_resume)
    {
        _on_resume(_task);
    }
    _origin = _current;
    _current = this;
    _ctx.swapIn();
    checkEnd();
}

void Coroutine::yield_naked()
{
    assert(_current == this);
    _state = STATE_WAITING;
    _current = _origin;
    _ctx.swapOut();
}

void Coroutine::resume_naked()
{
    assert(_current != this);
    if(_on_bailout)
    {
        return;
    }
    _state = STATE_RUNNING;
    _origin = _current;
    _current = this;
    _ctx.swapIn();
    checkEnd();
}

void Coroutine::close(){
    assert(_current == this);
    _state = STATE_END;
    if(_on_close)
    {
        _on_close(_task);
    }
    _current = _origin;
    _coroutines.erase(_cid);
    delete this;
}

void Coroutine::setOnYield(SwapCallback func)
{
    _on_yield = func;
}

void Coroutine::setOnResume(SwapCallback func)
{
    _on_resume = func;
}

void Coroutine::setOnClose(SwapCallback func)
{
    _on_close = func;
}

void Coroutine::bailout(BailoutCallback func)
{
    Coroutine *co = _current;
    if(!co)
    {
        _on_bailout = nullptr;
        return;
    }
    if(!func)
    {
        LOG_ERROR("");
    }
    if(!co->_task)
    {
        exit(255);
    }
    _on_bailout = func;
    while(co->_origin)
    {
        co = co->_origin;
    }
    co->yield();
    exit(1);
}

uint8_t coroutine_is_in()
{
    return !!Coroutine::getCurrent();
}

long coroutine_get_current_id()
{
    return Coroutine::getCurrentCid();
}
