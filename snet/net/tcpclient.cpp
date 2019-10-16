#include "tcpclient.h"
#include "socket.h"
#include "eventloop.h"
#include "channel.h"
#include "connector.h"
#include "tcpconnection.h"
#include "base/logmgr.h"

TcpClient::TcpClient(EventLoop *pLoop) : m_pLoop(pLoop), m_pConnection(), m_pConnector(new Connector(pLoop)), m_MsgCb(), m_bStarted(false), m_ConnedCb()
{
    m_pConnector->setNewConnectionCb(std::bind(&TcpClient::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}
TcpClient::~TcpClient()
{
}
void TcpClient::start(const std::string &strIp, uint16_t iPort)
{
    if (!m_bStarted.exchange(true))
    {
        m_pConnector->start(strIp, iPort);
    }
}
void TcpClient::setMsgCallBack(MsgCallBack cb)
{
    m_MsgCb = std::move(cb);
}
void TcpClient::setConnectedCallBack(ConnectedCallBack cb)
{
    m_ConnedCb = std::move(cb);
}

void TcpClient::newConnection(int32_t iFd, const InetAddr &addr)
{
    std::string strConnName = addr.toString();
    m_pConnection.reset(new TcpConnection(m_pLoop, strConnName, iFd, addr));
    LOG_DEBUG("new connection, name:" << strConnName);
    m_pConnection->setMsgCallBack(m_MsgCb);
    m_pConnection->setConnectedCallBack(m_ConnedCb);
    m_pLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, m_pConnection));
}

void TcpClient::send(const std::string &strMsg)
{
    m_pConnection->send(strMsg);
}