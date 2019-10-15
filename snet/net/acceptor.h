#ifndef __ACCEPTER_H__
#define __ACCEPTER_H__

#include "base/noncopyable.h"
#include <memory>
#include <string>
#include <functional>

class Socket;
class Channel;
class InetAddr;
class EventLoop;

class Acceptor : public NonCopyable
{
public:
    using newConnectionCallBack = std::function<void(int32_t iFd, InetAddr *addr)>;
    Acceptor(EventLoop *loop, const std::string &strIp, uint16_t iPort);
    ~Acceptor();
    void listen();
    void handleRead();
    void setNewConnectionCb(newConnectionCallBack cb);
    bool listening() { return m_bListening; }

private:
    EventLoop *m_pLoop;
    std::unique_ptr<Socket> m_pSock;
    std::unique_ptr<Channel> m_pChannel;
    bool m_bListening;
    newConnectionCallBack m_cb;
};
#endif