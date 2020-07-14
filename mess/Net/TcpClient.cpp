#include "TcpClient.h"
#include "EventLoop.h"
#include "Connector.h"
#include "Socket.h"
#include "TcpConnection.h"
#include "Log/Logger.h"

namespace detail
{
    void removeConnection(EventLoop *loop, const TcpConnectionPtr &conn)
    {
        loop->queueInLoop(std::bind(&TcpConnection::onConnDestroyed, conn));
    }
} // namespace detail

TcpClient::TcpClient(EventLoop *lp, const std::string &name1)
    : _loop(lp), _started(false), _connector(new Connector(lp)), _name(name1)
{
    _connector->setNewConnectionCb(std::bind(&TcpClient::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpClient::~TcpClient()
{
    if (_connection)
    {
        ConnectionCb cb = std::bind(&detail::removeConnection, _loop, std::placeholders::_1);
        auto closeCb = std::bind(&TcpConnection::setCloseCb, _connection, cb);
        _loop->runInLoop(closeCb);
        _connection->forceClose();
    }
    else
    {
        _connector->stop();
    }
}

void TcpClient::setMsgCb(MsgCb cb)
{
    _msgCb = std::move(cb);
}

void TcpClient::setConnCb(ConnectionCb cb)
{
    _connCb = std::move(cb);
}

void TcpClient::connect(const std::string &ip, uint16_t port)
{
    if (!_started.exchange(true))
    {
        _connector->start(ip, port);
    }
}

void TcpClient::disconnect()
{
    _started.exchange(false);
    _connection->shutdown();
}

void TcpClient::stop()
{
    _started.exchange(false);
    _connector->stop();
}

void TcpClient::send(const std::string &msg)
{
    _connection->send(msg);
}

void TcpClient::newConnection(int32_t fd, const InetAddr &addr)
{
    _connection.reset(new TcpConnection(_loop, fd, addr.toString(), addr));
    _connection->setConnedCb(_connCb);
    _connection->setMsgCb(_msgCb);
    _connection->setCloseCb(std::bind(&TcpClient::removeConnection, this, std::placeholders::_1));
    _loop->runInLoop(std::bind(&TcpConnection::onConnEstablished, _connection));
}

void TcpClient::removeConnection(const TcpConnectionPtr &conn)
{
    _connection.reset();
    _loop->queueInLoop(std::bind(&TcpConnection::onConnDestroyed, conn));
}