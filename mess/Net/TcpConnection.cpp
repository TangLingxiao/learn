#include "TcpConnection.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Log/Logger.h"
#include <unistd.h>

#define RECV_BUFSIZE (64 * 1024)
#define SEND_BUFSIZE (64 * 1024)

TcpConnection::TcpConnection(EventLoop *loop, int32_t fd, const std::string &cname, const InetAddr &addr)
    : _loop(loop), _name(cname), _peeraddr(addr), _channel(new Channel(loop, fd)), _sock(fd), _recvBuf(RECV_BUFSIZE), _sendBuf(SEND_BUFSIZE), _state(CONNECTING)
{
    _channel->setReadCb(std::bind(&TcpConnection::handleRead, this));
    _channel->setWriteCb(std::bind(&TcpConnection::handleWrite, this));
    _channel->setCloseCb(std::bind(&TcpConnection::handleClose, this));
    _channel->setErrorCb(std::bind(&TcpConnection::handleError, this));
    _sock.setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
    LOG_TRACE("TcpConnection released,name:" << _name);
}

void TcpConnection::setCloseCb(ConnectionCb cb)
{
    _closeCb = std::move(cb);
}

void TcpConnection::setConnedCb(ConnectionCb cb)
{
    _connedCb = std::move(cb);
}

void TcpConnection::setMsgCb(MsgCb cb)
{
    _msgCb = std::move(cb);
}

void TcpConnection::setPackMsgCb(PackMsgCb cb)
{
    _packCb = std::move(cb);
}

void TcpConnection::setParserMsgCb(ParserMsgCb cb)
{
    _parserCb = std::move(cb);
}

void TcpConnection::send(const std::string &msg)
{
    if (_state == CONNECTED)
    {
        _loop->runInLoop(std::bind(&TcpConnection::sendInLoop, shared_from_this(), msg));
    }
}

void TcpConnection::shutdown()
{
    if (_state == CONNECTED)
    {
        _state = DISCONNECTING;
        _loop->runInLoop(std::bind(&TcpConnection::shutdownInLoop, shared_from_this()));
    }
}

void TcpConnection::forceClose()
{
    if (_state == CONNECTED || _state == DISCONNECTING)
    {
        _state = DISCONNECTING;
        _loop->queueInLoop(std::bind(&TcpConnection::forceCloseInLoop, shared_from_this()));
    }
}

void TcpConnection::forceCloseInLoop()
{
    if (_state == CONNECTED || _state == DISCONNECTING)
    {
        handleClose();
    }
}

void TcpConnection::shutdownInLoop()
{
    if (!_channel->isWriting())
    {
        _sock.shutdownWrite();
    }
}

void TcpConnection::sendInLoop(const std::string &msg)
{
    if (_state == DISCONNECTED)
    {
        return;
    }
    if (_packCb)
    {
        _packCb(msg);
    }
    size_t remain = msg.size();
    ssize_t wrote = 0;
    if (_sendBuf.Empty() && !_channel->isWriting())
    {
        //try send directly
        wrote = ::write(_sock.fd(), msg.c_str(), msg.size());
        if (wrote >= 0)
        {
            remain = msg.size() - wrote;
        }
        else
        {
            LOG_ERROR("send error");
        }
    }

    if (remain > 0)
    {
        size_t size = _sendBuf.TotalSize() - _sendBuf.Size();
        if (size < remain)
        {
            _sendBuf.Realloc(_sendBuf.Size() + remain);
        }
        size = _sendBuf.TotalSize() - _sendBuf.Size();
        LIB_ASSERT(size >= remain, "error sendbuf size");
        _sendBuf.Write(msg.c_str() + wrote, remain);
        _channel->enableWriting();
    }
}

void TcpConnection::onConnEstablished()
{
    _channel->tie(shared_from_this());
    _channel->enableReading();
    _state = CONNECTED;
    if (_connedCb)
    {
        _connedCb(shared_from_this());
    }
    LOG_INFO("new connection established, peer:" << _peeraddr.toString());
}

void TcpConnection::onConnDestroyed()
{
    if (_state == CONNECTED)
    {
        _channel->disableAll();
        _state = DISCONNECTED;
    }
    _channel->remove();
    LOG_INFO("connection released, peer:" << _peeraddr.toString());
}

void TcpConnection::handleRead()
{
    size_t size{0};
    auto buf = _recvBuf.getCanWrite(size);
    if (size == 0)
    {
        _recvBuf.Realloc(2 * RECV_BUFSIZE);
    }
    buf = _recvBuf.getCanWrite(size);
    size = ::read(_sock.fd(), buf, size);
    _recvBuf.In(size);
    if (size > 0)
    {
        if (_parserCb)
        {
            size_t len{0};
            std::string msg;
            int32_t ret = _parserCb(_recvBuf, len, msg);
            if (ret == PACKEAGE_FULL)
            {
                _recvBuf.Out(len);
                if (_msgCb)
                {
                    _msgCb(shared_from_this(), msg);
                }
            }
            else if (ret == PACKEAGE_ERROR)
            {
                LOG_ERROR("recv msg error");
                _recvBuf.Clear();
            }
        }
        else
        {
            buf = _recvBuf.getCanRead(size);
            std::string msg{buf, size};
            _recvBuf.Out(size);
            if(_msgCb)
            {
                _msgCb(shared_from_this(), msg);
            }
        }
    }
    else if (size == 0)
    {
        handleClose();
    }
    else
    {
        LOG_ERROR("TcpConnection recv error");
    }
}

void TcpConnection::handleWrite()
{
    if (_channel->isWriting())
    {
        size_t size{0};
        auto buf = _sendBuf.getCanRead(size);
        size = ::write(_sock.fd(), buf, size);
        _sendBuf.Out(size);
        if (size > 0)
        {
            if (_sendBuf.Empty())
            {
                if (_sendBuf.TotalSize() > SEND_BUFSIZE)
                {
                    _sendBuf.Realloc(SEND_BUFSIZE);
                }
                _channel->disableWriting();
                if (_state == DISCONNECTING)
                {
                    shutdownInLoop();
                }
            }
        }
        else
        {
            LOG_ERROR("TcpConnection send error");
        }
    }
}

void TcpConnection::handleClose()
{
    _state = DISCONNECTED;
    _channel->disableAll();
    if (_closeCb)
    {
        _closeCb(shared_from_this());
    }
}

void TcpConnection::handleError()
{
    LOG_ERROR("handleerror");
}