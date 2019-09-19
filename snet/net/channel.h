#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <functional>
#include "base/noncopyable.h"
#include "base/callback.h"
class EventLoop;

class Channel : public NonCopyable
{
public:
    Channel(EventLoop *loop, int32_t iFd);
    void handleEvent();
    void setReadCallback(const EventCallback &cb) { m_fReadCallback = cb; }
    void setWriteCallback(const EventCallback &cb) { m_fWriteCallback = cb; }
    void setErrorCallback(const EventCallback &cb) { m_fErrorCallback = cb; }
    int32_t fd() { return m_iFd; }
    int32_t events() { return m_iEvents; }
    void setREvents(int32_t revt) { m_iREvents = revt; }
    bool isNoneEvent() { return m_iEvents == m_iNoneEvent; }

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

private:
    void update();
    static const int32_t m_iNoneEvent;
    static const int32_t m_iReadEvent;
    static const int32_t m_iWriteEvent;
    EventLoop *m_pLoop = nullptr;
    const int32_t m_iFd = -1;
    int32_t m_iEvents = 0;  // poller cares about
    int32_t m_iREvents = 0; // actually occurred
    int32_t m_index = -1;// poll vector index

    EventCallback m_fReadCallback = nullptr;
    EventCallback m_fWriteCallback = nullptr;
    EventCallback m_fErrorCallback = nullptr;
};
#endif