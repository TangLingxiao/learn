#pragma once
#include "Net/EventLoop.h"
#include "hiredis.h"
#include "async.h"
#include "Net/Channel.h"

struct redisLibeventEvents
{
    redisAsyncContext *context;
    EventLoop * loop;
    std::shared_ptr<Channel> channel;
};


static void redisLibeventAddRead(void *privdata) {
    redisLibeventEvents *e = (redisLibeventEvents*)privdata;
    struct EventLoop *loop = e->loop;
    ((void)loop);
    e->channel->enableReading();
}

static void redisLibeventDelRead(void *privdata) {
    redisLibeventEvents *e = (redisLibeventEvents*)privdata;
    struct EventLoop *loop = e->loop;
    ((void)loop);
    e->channel->disableReading();
}

static void redisLibeventAddWrite(void *privdata) {
    redisLibeventEvents *e = (redisLibeventEvents*)privdata;
    struct EventLoop *loop = e->loop;
    ((void)loop);
    e->channel->enableWriting();
}

static void redisLibeventDelWrite(void *privdata) {
    redisLibeventEvents *e = (redisLibeventEvents*)privdata;
    struct EventLoop *loop = e->loop;
    ((void)loop);
    e->channel->disableWriting();
}

static void redisLibeventCleanup(void *privdata) {
    redisLibeventEvents *e = (redisLibeventEvents*)privdata;
    struct EventLoop *loop = e->loop;
    ((void)loop);
    e->channel->disableAll();
    free(e);
}

static void handleClose(redisLibeventEvents *e)
{
    e->channel->disableAll();
}

static void handleRead(redisLibeventEvents *e)
{
    redisAsyncHandleRead(e->context);
}

static void handleWrite(redisLibeventEvents *e)
{
    redisAsyncHandleWrite(e->context);
}

static int redisLibeventAttach(redisAsyncContext *ac, EventLoop *base) {
    redisContext *c = &(ac->c);
    redisLibeventEvents *e;

    /* Nothing should be attached when something is already attached */
    if (ac->ev.data != NULL)
        return REDIS_ERR;

    /* Create container for context and r/w events */
    e = (redisLibeventEvents*)hi_calloc(1, sizeof(*e));
    if (e == NULL)
        return REDIS_ERR;

    e->context = ac;

    /* Register functions to start/stop listening for events */
    ac->ev.addRead = redisLibeventAddRead;
    ac->ev.delRead = redisLibeventDelRead;
    ac->ev.addWrite = redisLibeventAddWrite;
    ac->ev.delWrite = redisLibeventDelWrite;
    ac->ev.cleanup = redisLibeventCleanup;
    //ac->ev.scheduleTimer = redisLibeventSetTimeout;
    ac->ev.data = e;

    /* Initialize and install read/write events */
    std::shared_ptr<Channel> channel(new Channel(base, c->fd));
    e->channel = channel;
    channel->setCloseCb(std::bind(&handleClose, e));
    channel->setReadCb(std::bind(&handleRead, e));
    channel->setWriteCb(std::bind(&handleWrite, e));
    channel->setErrorCb(std::bind(&handleClose, e));
    channel->enableWriting();
    channel->enableReading();
    e->loop = base;
    return REDIS_OK;
}