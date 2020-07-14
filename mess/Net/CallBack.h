#pragma once
#include <functional>
#include <memory>
#include <string>

class InetAddr;
class TcpConnection;
class RingBuff;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using VoidCb = std::function<void()>;
using NewConnectionCb = std::function<void(int32_t, const InetAddr &)>;
using MsgCb = std::function<void(const TcpConnectionPtr &, const std::string &)>;
using ConnectionCb = std::function<void(const TcpConnectionPtr &)>;

using PackMsgCb = std::function<std::string(const std::string &)>;
using ParserMsgCb = std::function<int32_t(const RingBuff &, size_t &, std::string &)>;