#pragma once

#include <memory>
#include <stdint.h>
#include <string>
#include "CallBack.h"
#include "Pattern/NonCopyable.h"

class Socket;
class Channel;
class EventLoop;
class InetAddr;

class Acceptor : public NonCopyable
{
public:
    Acceptor(EventLoop *loop, const std::string &ip, uint16_t port);
    ~Acceptor();

    void setNewConnectionCb(NewConnectionCb callback);
    void listen();
    void handleRead();

private:
    EventLoop *_loop;
    std::unique_ptr<Socket> _sock;
    std::unique_ptr<Channel> _channel;
    NewConnectionCb _cb;
};