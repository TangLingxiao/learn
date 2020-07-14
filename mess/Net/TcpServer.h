#pragma once
#include "Pattern/NonCopyable.h"
#include <atomic>
#include <map>
#include "CallBack.h"

class EventLoop;
class TcpConnection;
class Acceptor;
class InetAddr;
class EventLoopThreadPool;
class TcpServer : public NonCopyable
{
public:
    TcpServer(EventLoop *lp, const std::string &n, const std::string &ip, uint16_t port);
    ~TcpServer();
    void start();
    void setThreadNum(int32_t num);
    void setMsgCb(MsgCb cb);
    void setConnCb(ConnectionCb cb);

    void newConnection(int32_t fd, const InetAddr &addr);

private:
    void disconn(const TcpConnectionPtr &conn);
    void removeConn(const TcpConnectionPtr &conn);

private:
    EventLoop *_loop;
    std::unique_ptr<Acceptor> _acceptor;
    std::map<std::string, TcpConnectionPtr> _conns;
    std::atomic<bool> _started;
    std::string _name;
    std::unique_ptr<EventLoopThreadPool> _pool;
    MsgCb _msgCb;
    ConnectionCb _connCb;
};