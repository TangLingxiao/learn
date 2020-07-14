#include "EventLoop.h"
#include "Socket.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "EventLoopThreadPool.h"
#include "Log/Logger.h"

TcpServer::TcpServer(EventLoop *lp, const std::string &n, const std::string &ip, uint16_t port)
    : _loop(lp), _acceptor(new Acceptor(lp, ip, port)), _started(false), _name(n), _pool(new EventLoopThreadPool(lp))
{
    _acceptor->setNewConnectionCb(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer()
{
    for (auto &item : _conns)
    {
        TcpConnectionPtr conn(item.second);
        item.second.reset();
        conn->getLoop()->runInLoop(std::bind(&TcpConnection::onConnDestroyed, conn));
    }
}

void TcpServer::start()
{
    if (!_started.exchange(true))
    {
        _pool->start();
        _loop->runInLoop(std::bind(&Acceptor::listen, this->_acceptor.get()));
    }
}

void TcpServer::setThreadNum(int32_t num)
{
    _pool->setThreadNum(num);
}

void TcpServer::setMsgCb(MsgCb cb)
{
    _msgCb = std::move(cb);
}

void TcpServer::setConnCb(ConnectionCb cb)
{
    _connCb = std::move(cb);
}

void TcpServer::newConnection(int32_t fd, const InetAddr &addr)
{
    std::string name = _name + addr.toString();
    auto loop = _pool->getLoop();
    TcpConnectionPtr conn(new TcpConnection(loop, fd, name, addr));
    _conns[name] = conn;
    conn->setConnedCb(_connCb);
    conn->setMsgCb(_msgCb);
    conn->setCloseCb(std::bind(&TcpServer::disconn, this, std::placeholders::_1));
    loop->runInLoop(std::bind(&TcpConnection::onConnEstablished, conn));
}

void TcpServer::disconn(const TcpConnectionPtr &conn)
{
    _loop->runInLoop(std::bind(&TcpServer::removeConn, this, conn));
}

void TcpServer::removeConn(const TcpConnectionPtr &conn)
{
    const auto &name = conn->name();
    _conns.erase(name);
    auto loop = conn->getLoop();
    loop->queueInLoop(std::bind(&TcpConnection::onConnDestroyed, conn));
}