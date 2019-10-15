#include "tcpconnection.h"
#include "channel.h"
#include "eventloop.h"
#include <cassert>
#include <unistd.h>
#include "base/logmgr.h"

TcpConnection::TcpConnection(EventLoop *pLoop, const std::string &strName, int32_t iFd, const InetAddr &oAddr)
    : m_pLoop(pLoop), m_strName(strName), m_oPeerAddr(oAddr), m_pChannel(new Channel(pLoop, iFd)), m_oSock(iFd), m_strRecvBuf(), m_strSendBuf(), m_MsgCb(), m_CloseCb()
{
    m_pChannel->setReadCallback(std::bind(&TcpConnection::handleRead, this));
    m_oSock.setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
    LOG_DEBUG("TcpConnection released,name:" << m_strName);
}

void TcpConnection::setMsgCallBack(MsgCallBack cb)
{
    m_MsgCb = std::move(cb);
}

void TcpConnection::setCloseCallBack(CloseCallBack cb)
{
    m_CloseCb = std::move(cb);
}

void TcpConnection::send(const std::string &strMsg)
{
    auto n = ::write(m_oSock.fd(), strMsg.c_str(), strMsg.size());
    if (n != strMsg.size())
    {
        LOG_ERROR("TcpConnection send error");
    }
}

void TcpConnection::connectEstablished()
{
    assert(m_pLoop->inLoopThread());

    m_pChannel->tie(shared_from_this());
    m_pChannel->enableReading();
}

void TcpConnection::handleRead()
{
    char buf[65536];
    auto n = ::read(m_oSock.fd(), buf, sizeof buf);
    if (n > 0)
    {
        if (m_MsgCb)
        {
            m_MsgCb(shared_from_this(), std::string(buf, n));
        }
    }
    else if (n == 0)
    {
        handleClose();
    }
}

void TcpConnection::handleClose()
{
    if (m_CloseCb)
    {
        m_CloseCb(shared_from_this());
    }
}

void TcpConnection::connectDestroyed()
{
    assert(m_pLoop->inLoopThread());
    m_pChannel->disableAll();
    m_pChannel->remove();
}