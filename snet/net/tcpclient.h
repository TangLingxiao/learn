#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__

#include <memory>
#include <string>
#include "base/callback.h"
#include "base/noncopyable.h"

class Connector;
class EventLoop;
class Channel;

class TcpClient : public NonCopyable
{
public:
    TcpClient(EventLoop *pLoop);
    ~TcpClient();
    void start(const std::string &strIp, uint16_t iPort);
    void setMsgCallBack(MsgCallBack cb);
    void close();

private:
    EventLoop *m_pLoop;
    TcpConnectionPtr m_pConnection;
    std::unique_ptr<Connector> m_pConnector;
    MsgCallBack m_MsgCb;
};

#endif