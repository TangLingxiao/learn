#include "channel.h"
#include "base/logmgr.h"
#include <poll.h>
#include "eventloop.h"

const int32_t Channel::m_iNoneEvent = 0;
const int32_t Channel::m_iReadEvent = POLLIN | POLLPRI;
const int32_t Channel::m_iWriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int32_t iFd)
    : m_iFd(iFd)
{
    m_pLoop = loop;
}

void Channel::update()
{
    m_pLoop->updateChannel(this);
}
void Channel::remove()
{
    m_pLoop->removeChannel(this);
}
void Channel::handleEvent()
{
    if (m_bTied)
    {
        auto pTie = m_pTie.lock();
        if (pTie)
        {
            handleEventWithGuard();
        }
    }
    else
    {
        handleEventWithGuard();
    }
}
void Channel::handleEventWithGuard()
{
    if (m_iREvents & POLLNVAL)
    {
        LOG_WARN("Channel::handleEvent POLLNVAL");
    }
    if (m_iREvents & (POLLERR | POLLNVAL))
    {
        if (m_fErrorCallback)
        {
            m_fErrorCallback();
        }
    }
    if (m_iREvents & (POLLIN | POLLPRI | POLLRDHUP))
    {
        if (m_fReadCallback)
        {
            m_fReadCallback();
        }
    }
    if (m_iREvents & POLLOUT)
    {
        if (m_fWriteCallback)
        {
            m_fWriteCallback();
        }
    }
}

void Channel::tie(const std::shared_ptr<void> &pTie)
{
    m_bTied = true;
    m_pTie = pTie;
}
