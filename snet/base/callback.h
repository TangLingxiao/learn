#ifndef __CALLBACK_H__
#define __CALLBACK_H__
#include <functional>
#include <memory>
#include <string>
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TimerCallBack = std::function<void()>;
using EventCallBack = std::function<void()>;
using MsgCallBack = std::function<void(const TcpConnectionPtr &, const std::string &)>;
using CloseCallBack = std::function<void(const TcpConnectionPtr &)>;
#endif