#include "acceptor.h"
#include "socket.h"
#include "channel.h"
#include "eventloop.h"
#include "base/logmgr.h"
#include <cassert>
#include <sys/socket.h>
#include <memory.h>
#include <unistd.h>
#include <netinet/in.h>

Acceptor::Acceptor(EventLoop *loop, const std::string &strIp, uint16_t iPort)
    : m_pLoop(loop), m_pSock(new Socket(createSocketfd())), m_pChannel(new Channel(m_pLoop, m_pSock->fd())), m_bListening(false)
{
    m_pSock->setReuse(true);
    m_pSock->setTcpNoDelay(true);
    assert(m_pSock->bind(InetAddr(strIp, iPort)));
    m_pChannel->setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
}

void Acceptor::listen()
{
    assert(m_pLoop->inLoopThread());
    m_bListening = true;
    m_pSock->listen();
    m_pChannel->enableReading();
}
void Acceptor::handleRead()
{
    assert(m_pLoop->inLoopThread());
    InetAddr oAddr;
    int32_t iFd = m_pSock->accept(&oAddr);
    if (iFd > 0)
    {
        if (m_cb)
        {
            m_cb(iFd, &oAddr);
            LOG_INFO("new connection," << oAddr.toString());
        }
        else
        {
            ::close(iFd);
        }
    }
    else
    {
        LOG_ERROR("acceptor handleread error");
    }
}
void Acceptor::setNewConnectionCb(newConnectionCallBack cb)
{
    m_cb = std::move(cb);
}