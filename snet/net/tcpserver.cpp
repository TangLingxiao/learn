#include "eventloop.h"
#include "acceptor.h"
#include "socket.h"
#include "base/logmgr.h"
#include "tcpserver.h"
#include "tcpconnection.h"
#include <cassert>

TcpServer::TcpServer(EventLoop *loop, const std::string &strIp, uint16_t iPort, const std::string &strName)
    : m_pLoop(loop), m_pAcceptor(new Acceptor(loop, strIp, iPort)), m_bStarted(false), m_strName(strName), m_mapConnections(), m_MsgCb(), m_ConnedCb()
{
    m_pAcceptor->setNewConnectionCb(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer()
{
}

void TcpServer::start()
{
    if (!m_bStarted.exchange(true))
    {
        m_pLoop->runInLoop(std::bind(&Acceptor::listen, m_pAcceptor.get()));
    }
}

void TcpServer::setMsgCallBack(MsgCallBack cb)
{
    m_MsgCb = std::move(cb);
}
void TcpServer::setConnectedCallBack(ConnectedCallBack cb)
{
    m_ConnedCb = std::move(cb);
}
void TcpServer::newConnection(int32_t iFd, const InetAddr &addr)
{
    std::string strConnName = m_strName + addr.toString();
    TcpConnectionPtr pConnection(new TcpConnection(m_pLoop, strConnName, iFd, addr));
    m_mapConnections[strConnName] = pConnection;
    LOG_DEBUG("new connection, name:" << strConnName);
    pConnection->setMsgCallBack(m_MsgCb);
    pConnection->setCloseCallBack(std::bind(&TcpServer::disconnect, this, std::placeholders::_1));
    m_pLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, pConnection));
}

void TcpServer::disconnect(const TcpConnectionPtr &pConn)
{
    m_pLoop->runInLoop(std::bind(&TcpServer::removeConnection, this, pConn));
}

void TcpServer::removeConnection(const TcpConnectionPtr &pConn)
{
    assert(m_pLoop->inLoopThread());
    auto pLoop = pConn->getLoop();
    const auto &strName = pConn->getName();
    m_mapConnections.erase(strName);
    LOG_INFO("removeConnection,name:" << strName);
    pLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, pConn));
}