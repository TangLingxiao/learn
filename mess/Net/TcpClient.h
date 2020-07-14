#pragma once

#include <memory>
#include "CallBack.h"
#include "Pattern/NonCopyable.h"
#include <atomic>

class EventLoop;
class Connector;
class InetAddr;
class TcpConnection;

class TcpClient : public NonCopyable
{
public:
    TcpClient(EventLoop *lp, const std::string& name);
    ~TcpClient();
    void setMsgCb(MsgCb cb);
    void setConnCb(ConnectionCb cb);
    void connect(const std::string &ip, uint16_t port);
    void disconnect();
    void stop();
    void send(const std::string &msg);

private:
    void newConnection(int32_t fd, const InetAddr &addr);
    void removeConnection(const TcpConnectionPtr& conn);
private:
    EventLoop *_loop;
    std::atomic<bool> _started;
    std::unique_ptr<Connector> _connector;
    std::string _name;
    TcpConnectionPtr _connection;
    MsgCb _msgCb;
    ConnectionCb _connCb;
};