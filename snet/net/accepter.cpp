#include "accepter.h"
#include "socket.h"
#include "channel.h"
#include "eventloop.h"
#include "base/logmgr.h"
#include <cassert>
#include <sys/socket.h>
#include <memory.h>
#include <unistd.h>
#include <netinet/in.h>

Accepter::Accepter(EventLoop *loop, const std::string &strIp, uint16_t iPort)
    : m_pLoop(loop), m_pSock(new Socket(createSocketfd())), m_pChannel(new Channel(m_pLoop, m_pSock->fd()))
{
    m_pSock->setReuse(true);
    m_pSock->setTcpNoDelay(true);
    assert(m_pSock->bind(strIp, iPort));
    m_pChannel->setReadCallback(std::bind(&Accepter::handleRead, this));
}
Accepter::~Accepter()
{
}

void Accepter::listen()
{
    m_pSock->listen();
    m_pChannel->enableReading();
}
void Accepter::handleRead()
{
    assert(m_pLoop->inLoopThread());
    sockaddr_in oAddr;
    memset(&oAddr, 0, sizeof oAddr);
    int32_t iFd = m_pSock->accept(&oAddr);
    if (iFd > 0)
    {
        if (m_cb)
        {
            m_cb(iFd, &oAddr);
            logNewConnection(&oAddr);
        }
        else
        {
            ::close(iFd);
        }
    }
    else
    {
        LOG_ERROR("accepter handleread error");
    }
}
void Accepter::setNewConnectionCb(newConnectionCallBack cb)
{
    m_cb = std::move(cb);
}