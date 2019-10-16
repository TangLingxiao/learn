#include "connector.h"
#include "socket.h"
#include "eventloop.h"
#include "channel.h"
#include <cassert>
#include "base/logmgr.h"
#include <unistd.h>

Connector::Connector(EventLoop *loop) : m_pLoop(loop), m_pChannel(new Channel(m_pLoop, -1)), m_cb()
{
}

Connector::~Connector()
{
}

void Connector::setNewConnectionCb(newConnectionCallBack cb)
{
    m_cb = std::move(cb);
}

void Connector::start(const std::string &strIp, uint16_t iPort)
{
    assert(m_pLoop != nullptr);
    m_pLoop->runInLoop(std::bind(&Connector::connect, this, InetAddr(strIp, iPort)));
}

void Connector::connect(const InetAddr &oAddr)
{
    assert(m_pLoop != nullptr);
    assert(m_pLoop->inLoopThread());
    int32_t iFd = createSocketfd();
    LOG_DEBUG("fd:" << iFd << ", addr " << oAddr.toString());
    if (sockets::connect(iFd, oAddr))
    {
        m_pChannel.reset(new Channel(m_pLoop, iFd));
        m_pChannel->setWriteCallback(std::bind(&Connector::handleWrite, this, oAddr));
        m_pChannel->setErrorCallback(std::bind(&Connector::handleWrite, this, oAddr));
        m_pChannel->enableWriting();
    }
}

int32_t Connector::removeAndResetChannel()
{
    m_pChannel->disableAll();
    m_pChannel->remove();
    int32_t iFd = m_pChannel->fd();
    m_pLoop->queueInLoop(std::bind(&Connector::resetChannel, this));
    return iFd;
}

void Connector::resetChannel()
{
    m_pChannel.reset();
}

void Connector::handleWrite(const InetAddr &addr)
{
    if (m_cb)
    {
        int32_t iFd = removeAndResetChannel();
        m_cb(iFd, addr);
    }
}