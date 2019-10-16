#ifndef __CONNETOR_H__
#define __CONNETOR_H__

#include "base/noncopyable.h"
#include <memory>
#include <functional>
#include <string>
#include "base/callback.h"

// class Socket;
class Channel;
class InetAddr;
class EventLoop;

class Connector : public NonCopyable
{
public:
    Connector(EventLoop *loop);
    ~Connector();
    void setNewConnectionCb(newConnectionCallBack cb);
    void start(const std::string &strIp, uint16_t iPort);
    void handleWrite(const InetAddr &addr);

private:
    void connect(const InetAddr &oAddr);
    int32_t removeAndResetChannel();
    void resetChannel();
private:
    EventLoop *m_pLoop;
    // std::unique_ptr<Socket> m_pSock;
    std::unique_ptr<Channel> m_pChannel;
    newConnectionCallBack m_cb;
};

#endif