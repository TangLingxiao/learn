#include "tcpconnection.h"
#include "channel.h"
#include "eventloop.h"
#include <cassert>
#include <unistd.h>
#include "base/logmgr.h"

TcpConnection::TcpConnection(EventLoop *pLoop, const std::string &strName, int32_t iFd, const InetAddr &oAddr)
    : m_pLoop(pLoop), m_strName(strName), m_oPeerAddr(oAddr), m_pChannel(new Channel(pLoop, iFd)), m_oSock(iFd), m_strRecvBuf(), m_strSendBuf(), m_MsgCb(), m_CloseCb(), m_ConnedCb()
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

void TcpConnection::setConnectedCallBack(ConnectedCallBack cb)
{
    m_ConnedCb = std::move(cb);
}

void TcpConnection::send(const std::string &strMsg)
{
    m_pLoop->runInLoop(std::bind(&TcpConnection::sendInLoop, shared_from_this(), strMsg));
}

void TcpConnection::sendInLoop(const std::string &strMsg)
{
    assert(m_pLoop->inLoopThread());
    size_t iRemain = 0;
    ssize_t iWrote = 0;
    if (m_strSendBuf.empty() && !m_pChannel->isWriting())
    {
        iWrote = ::write(m_oSock.fd(), strMsg.c_str(), strMsg.size());
        if (iWrote >= 0)
        {
            iRemain = strMsg.size() - iWrote;
        }
        else
        {
            LOG_ERROR("write error");
        }
    }
    assert(iRemain <= strMsg.size());
    if (iRemain > 0)
    {
        m_strSendBuf.append(strMsg.c_str() + iWrote, iRemain);
        if (!m_pChannel->isWriting())
        {
            m_pChannel->enableWriting();
        }
    }
}

void TcpConnection::connectEstablished()
{
    assert(m_pLoop->inLoopThread());

    m_pChannel->tie(shared_from_this());
    m_pChannel->enableReading();
    if (m_ConnedCb)
    {
        m_ConnedCb(shared_from_this());
    }
}

void TcpConnection::handleRead()
{
    assert(m_pLoop->inLoopThread());
    char buf[65536];
    auto n = ::read(m_oSock.fd(), buf, sizeof buf);
    if (n > 0)
    {
        m_strRecvBuf.append(buf, n);
        if (m_MsgCb)
        {
            m_MsgCb(shared_from_this(), m_strRecvBuf);
        }
    }
    else if (n == 0)
    {
        handleClose();
    }
    else
    {
        LOG_ERROR("tcpconnection recv error");
    }
}

void TcpConnection::handleWrite()
{
    assert(m_pLoop->inLoopThread());
    if (m_pChannel->isWriting())
    {
        size_t n = ::write(m_oSock.fd(), m_strSendBuf.c_str(), m_strSendBuf.size());
        if (n > 0)
        {
            m_strSendBuf.erase(m_strSendBuf.begin(), m_strSendBuf.begin() + n);
            if (m_strSendBuf.empty())
            {
                m_pChannel->disableWriting();
            }
        }
        else
        {
            LOG_ERROR("tcpconnection send error");
        }
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