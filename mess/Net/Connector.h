#pragma once

#include "CallBack.h"
#include <memory>
#include "Pattern/NonCopyable.h"
#include "Socket.h"

class Socket;
class Channel;
class EventLoop;

class Connector : public NonCopyable
{
public:
    Connector(EventLoop *loop);
    ~Connector();

    void setNewConnectionCb(NewConnectionCb callback);
    void start(const std::string& ip, uint16_t port);
    void restart();
    void stop();
private:
    void resetChannel();
    void connect();
    void connecting(int32_t fd);
    int32_t removeAndResetChannel();
    void retry(int32_t fd);
    void handleWrite();
    void handleError();
    void stopInLoop();
private:
    enum State{DISCONNECTED, CONNECTING, CONNECTED};
    EventLoop *_loop;
    std::unique_ptr<Channel> _channel;
    InetAddr _svraddr;
    State _state;
    bool _connect;
    int64_t _retryDelayMs;
    NewConnectionCb _cb;
};