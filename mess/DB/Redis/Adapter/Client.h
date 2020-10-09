#pragma once
#include "Async.h"
#include <string>
#include <deque>
#include <mutex>
#include "Pattern/Singleton.h"
#include "Log/Logger.h"
#include "DB/Redis/RedisUtil.h"
struct coParam
{
    long cid;
    redisReply *reply;
    redisAsyncContext *c;
    ~coParam()
    {
        LOG_DEBUG("release");
    }
};
class Co_RedisClient :public Singleton<Co_RedisClient>
{
public:
    bool init(EventLoop *loop);
    void execCommand(void *argv);
  
private:
    redisAsyncContext *ctx;
    std::deque<coParam*> qu;
    std::mutex lk;
};