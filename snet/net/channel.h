#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <functional>
#include "base/noncopyable.h"
#include "base/callback.h"
#include <memory>
class EventLoop;

class Channel : public NonCopyable
{
public:
    enum EpollChannelState
    {
        E_NEW = -1,
        E_ADDED = 1,
        E_DELETED = 2,
    };
    Channel(EventLoop *loop, int32_t iFd);
    void handleEvent();
    void setReadCallback(EventCallBack cb) { m_fReadCallback = std::move(cb); }
    void setWriteCallback(EventCallBack cb) { m_fWriteCallback = std::move(cb); }
    void setErrorCallback(EventCallBack cb) { m_fErrorCallback = std::move(cb); }
    int32_t fd() { return m_iFd; }
    int32_t events() { return m_iEvents; }
    void setREvents(int32_t revt) { m_iREvents = revt; }
    bool isNoneEvent() { return m_iEvents == m_iNoneEvent; }
    bool isReading() { return m_iEvents & m_iReadEvent; }
    bool isWriting() { return m_iEvents & m_iWriteEvent; }
    void enableReading()
    {
        m_iEvents |= m_iReadEvent;
        update();
    }
    void disableReading()
    {
        m_iEvents &= ~m_iReadEvent;
        update();
    }
    void enableWriting()
    {
        m_iEvents |= m_iWriteEvent;
        update();
    }
    void disableWriting()
    {
        m_iEvents &= ~m_iWriteEvent;
        update();
    }
    void disableAll()
    {
        m_iEvents = m_iNoneEvent;
        update();
    }

    int32_t index() { return m_index; }
    void setIndex(int32_t idx) { m_index = idx; }
    EventLoop *getEventLoop() { return m_pLoop; }

    void tie(const std::shared_ptr<void> &pTie);
    void remove();

private:
    void update();
    void handleEventWithGuard();
    static const int32_t m_iNoneEvent;
    static const int32_t m_iReadEvent;
    static const int32_t m_iWriteEvent;
    EventLoop *m_pLoop = nullptr;
    const int32_t m_iFd = -1;
    int32_t m_iEvents = 0;  // poller cares about
    int32_t m_iREvents = 0; // actually occurred
    int32_t m_index = -1;   // poll vector index

    bool m_bTied = false;
    std::weak_ptr<void> m_pTie{};
    EventCallBack m_fReadCallback = nullptr;
    EventCallBack m_fWriteCallback = nullptr;
    EventCallBack m_fErrorCallback = nullptr;
};
#endif