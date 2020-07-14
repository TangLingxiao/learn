#pragma once

#include <memory>
#include <functional>
#include "CallBack.h"
#include "Pattern/NonCopyable.h"

class EventLoop;
class Channel : public NonCopyable
{
public:
    Channel(EventLoop *loop, int32_t fd);
    void handleEventWithGuard();
    void setReadCb(VoidCb cb) { _readCb = std::move(cb); }
    void setWriteCb(VoidCb cb) { _writeCb = std::move(cb); }
    void setErrorCb(VoidCb cb) { _errorCb = std::move(cb); }
    void setCloseCb(VoidCb cb) { _closeCb = std::move(cb); }
    int32_t fd() { return _fd; }
    int32_t events() { return _events; }
    void setRevents(int32_t revt) { _revents = revt; }
    bool isNoneEvents() { return _events == _noneEvent; }
    bool isReading() { return _events & _readEvent; }
    bool isWriting() { return _events & _writeEvent; }
    int32_t index() { return _index; }
    void setIndex(int32_t id) { _index = id; }
    void enableReading()
    {
        _events |= _readEvent;
        update();
    }

    void enableWriting()
    {
        _events |= _writeEvent;
        update();
    }

    void disableReading()
    {
        _events &= ~_readEvent;
        update();
    }

    void disableWriting()
    {
        _events &= ~_writeEvent;
        update();
    }

    void disableAll()
    {
        _events = _noneEvent;
        update();
    }

    void remove();

    void tie(const std::shared_ptr<void> &ptr);

private:
    void update();
    void handleEvent();

private:
    EventLoop *_loop{nullptr};
    const int32_t _fd{-1};
    int32_t _events{0};       //poller cares about
    int32_t _revents{0};      //actually occured
    int32_t _index{-1};       //poll vector index
    std::weak_ptr<void> _tie; //保存一个connection的弱引用防止handleevent时connection断开
    bool _tied{false};
    VoidCb _readCb{nullptr};
    VoidCb _writeCb{nullptr};
    VoidCb _errorCb{nullptr};
    VoidCb _closeCb{nullptr};
    static const int32_t _noneEvent;
    static const int32_t _readEvent;
    static const int32_t _writeEvent;
};