#pragma once

#include <string>
#include <memory>
#include "CallBack.h"
#include "Pattern/NonCopyable.h"
#include "Socket.h"
#include "Structure/RingBuffer.h"

class Channel;
class EventLoop;
class Socket;
class InetAddr;

class TcpConnection : public NonCopyable, public std::enable_shared_from_this<TcpConnection>
{
    enum
    {
        PACKEAGE_ERROR = -1,
        PACKEAGE_FULL = 0,
        PACKEAGE_LESS = 1,
    };

public:
    TcpConnection(EventLoop *loop, int32_t fd, const std::string &cname, const InetAddr &addr);
    ~TcpConnection();

    const std::string &name() { return _name; }
    void setCloseCb(ConnectionCb cb);
    void setConnedCb(ConnectionCb cb);
    void setMsgCb(MsgCb cb);
    void setPackMsgCb(PackMsgCb cb);
    void setParserMsgCb(ParserMsgCb cb);

    void send(const std::string &msg);
    void shutdown();
    void forceClose();
    void onConnEstablished();
    void onConnDestroyed();
    EventLoop *getLoop() { return _loop; }

    std::string getPeer(){return _peeraddr.toString();}
private:
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleError();
    void sendInLoop(const std::string &msg);
    void shutdownInLoop();
    void forceCloseInLoop();

private:
    enum State
    {
        CONNECTING,
        CONNECTED,
        DISCONNECTING,
        DISCONNECTED
    };
    EventLoop *_loop{nullptr};
    const std::string _name;
    const InetAddr _peeraddr;
    std::unique_ptr<Channel> _channel;
    Socket _sock;
    RingBuff _recvBuf;
    RingBuff _sendBuf;
    State _state;
    ConnectionCb _closeCb;
    ConnectionCb _connedCb;
    MsgCb _msgCb;
    PackMsgCb _packCb;
    ParserMsgCb _parserCb;
};
