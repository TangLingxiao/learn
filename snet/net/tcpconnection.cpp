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

void TcpConnection::packMsg(const std::string &strMsg, std::string &strMsgOut)
{
    uint32_t iHeaderLen = static_cast<uint32_t>(sizeof(uint32_t) + strMsg.size());
    strMsgOut.reserve(iHeaderLen);
    iHeaderLen = htonl(iHeaderLen);
    strMsgOut.append((char *)&iHeaderLen, sizeof iHeaderLen);
    strMsgOut.append(strMsg);
}

int32_t TcpConnection::parserMsg(uint32_t &iHeaderLen, std::string &strMsg)
{
    size_t iLen = m_strRecvBuf.size();
    if (iLen < sizeof(uint32_t))
    {
        return PACKEAGE_LESS;
    }
    iHeaderLen = ntohl(*(uint32_t *)m_strRecvBuf.c_str());
    if (iHeaderLen < sizeof(uint32_t) || iHeaderLen > 100 * 1024 * 1024)
    {
        return PACKEAGE_ERROR;
    }
    if (iLen < iHeaderLen)
    {
        return PACKEAGE_LESS;
    }
    strMsg.assign(m_strRecvBuf.c_str() + sizeof(uint32_t), m_strRecvBuf.c_str() + iHeaderLen);
    return PACKEAGE_FULL;
}

void TcpConnection::handleRead()
{
    assert(m_pLoop->inLoopThread());
    char buf[65536];
    auto n = ::read(m_oSock.fd(), buf, sizeof buf);
    if (n > 0)
    {
        m_strRecvBuf.append(buf, n);
        uint32_t iHeaderLen = 0;
        std::string strMsg;
        int32_t iRet = parserMsg(iHeaderLen, strMsg);
        if (iRet == PACKEAGE_FULL)
        {
            m_strRecvBuf.erase(m_strRecvBuf.begin(), m_strRecvBuf.begin() + iHeaderLen);
            if (m_MsgCb)
            {
                m_MsgCb(shared_from_this(), strMsg);
            }
        }
        else if (iRet == PACKEAGE_ERROR)
        {
            LOG_ERROR("tcpconnection recv msg error");
            m_strRecvBuf.clear();
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