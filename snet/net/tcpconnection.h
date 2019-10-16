#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include <memory>
#include <functional>
#include "base/noncopyable.h"
#include "socket.h"
#include <string>
#include "base/callback.h"

class Channel;
class EventLoop;
class TcpConnection : public NonCopyable, public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(EventLoop *pLoop, const std::string &strName, int32_t iFd, const InetAddr &oAddr);
    ~TcpConnection();
    void setMsgCallBack(MsgCallBack cb);
    void setCloseCallBack(CloseCallBack cb);
    void setConnectedCallBack(ConnectedCallBack cb);
    void send(const std::string& strMsg);
    EventLoop *getLoop() { return m_pLoop; }
    const std::string &getName() { return m_strName; }
    void connectEstablished();
    void connectDestroyed();

private:
    void handleRead();
    void handleClose();
    void sendInLoop(const std::string& strMsg);
private:
    EventLoop *m_pLoop;
    const std::string m_strName;
    const InetAddr m_oPeerAddr;
    std::unique_ptr<Channel> m_pChannel;
    Socket m_oSock;
    std::string m_strRecvBuf;
    std::string m_strSendBuf;
    MsgCallBack m_MsgCb;
    CloseCallBack m_CloseCb;
    ConnectedCallBack m_ConnedCb;
};
#endif