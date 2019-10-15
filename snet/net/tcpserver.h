#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include <atomic>
#include <memory>
#include "base/noncopyable.h"
#include <map>
#include <string>
#include "base/callback.h"

class InetAddr;
class Acceptor;
class EventLoop;
class TcpServer : public NonCopyable
{
public:
    TcpServer(EventLoop *loop, const std::string &strIp, uint16_t iPort, const std::string &strName);
    ~TcpServer();
    void start();
    void setMsgCallBack(MsgCallBack cb);
    void disconnect(const TcpConnectionPtr & pConn);
private:
    void newConnection(int32_t iFd, InetAddr *addr);
    void removeConnection(const TcpConnectionPtr & pConn);
    EventLoop *m_pLoop;
    std::unique_ptr<Acceptor> m_pAcceptor;
    std::atomic<bool> m_bStarted;
    std::string m_strName;
    std::map<std::string, TcpConnectionPtr> m_mapConnections;
    MsgCallBack m_MsgCb;
};
#endif