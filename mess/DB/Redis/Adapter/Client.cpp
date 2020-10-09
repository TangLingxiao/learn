#include "Client.h"
#include "RedisUtil.h"
//#include "pt.h"
#include <functional>
#include "Coroutine/Coroutine.h"
#include <unistd.h>
#include <thread>


void getCallback(redisAsyncContext *c, void *r, void *privdata)
{
    coParam *p = (coParam*)privdata;
    p->c = c;
    p->reply = (redisReply*)r;
    LOG_DEBUG("cid:"<<p->cid<<"resume, thread:"<<std::this_thread::get_id());
    Coroutine::getByCid(p->cid)->resume();
}
void connectCallback(const redisAsyncContext *c, int status)
{
    if (status != REDIS_OK)
    {
        printf("Error: %s\n", c->errstr);
        return;
    }
    printf("Connected...\n");
}

void disconnectCallback(const redisAsyncContext *c, int status)
{
    if (status != REDIS_OK)
    {
        printf("Error: %s\n", c->errstr);
        return;
    }
    printf("Disconnected...\n");
}

bool Co_RedisClient::init(EventLoop *loop)
{
    ctx = redisAsyncConnect("127.0.0.1", 6379);
    if (ctx->err)
    {
        /* Let *c leak for now... */
        printf("Error: %s\n", ctx->errstr);
        return false;
    }
    redisLibeventAttach(ctx, loop);
    redisAsyncSetConnectCallback(ctx, connectCallback);
    redisAsyncSetDisconnectCallback(ctx, disconnectCallback);
    return true;
}


void Co_RedisClient::execCommand(void *cmd)
{
    CmdArg &argv = *( CmdArg *)cmd;
    coParam pa;
    [&]()
    {
        pa.cid = Coroutine::getCurrentCid();
        redisAsyncCommandArgv(ctx, getCallback, &pa,argv.size(), argv.argv(),argv.argv_len());
        LOG_DEBUG("cid:"<<pa.cid<<"yield, thread:"<<std::this_thread::get_id());
        Coroutine::getByCid(Coroutine::getCurrentCid())->yield();
    }();
    LOG_DEBUG("execCommand:"<<pa.reply->str);
    LOG_DEBUG("cid:"<<pa.cid<<"resumed, thread:"<<std::this_thread::get_id());
}