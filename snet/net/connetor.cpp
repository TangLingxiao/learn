#include "connector.h"
#include "socket.h"
#include "eventloop.h"
#include "channel.h"
#include <cassert>
#include "base/logmgr.h"

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
    Socket sock(createSocketfd());
    LOG_DEBUG("fd:" << sock.fd() << ", addr " << oAddr.toString());
    if (sock.connect(oAddr))
    {
        m_pChannel.reset(new Channel(m_pLoop, sock.fd()));
        m_pChannel->setWriteCallback(std::bind(&Connector::handleWrite, this));
        m_pChannel->setErrorCallback(std::bind(&Connector::handleWrite, this));
        m_pChannel->enableWriting();
    }
}

void Connector::handleWrite()
{
    if (m_cb)
    {
        m_cb();
    }
}