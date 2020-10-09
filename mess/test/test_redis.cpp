#include "DB/Redis/RedisClient.h"
#include "Net/Application.h"
#include "Log/Logger.h"
#include "Util/TimeHelper.h"
#include "3rdparty/include/libco/co_routine.h"
#include "DB/Redis/Test/BenchMark.h"
#include <thread>
#include "DB/Redis/Adapter/Client.h"
#include "Net/EventLoopThread.h"
#include "Coroutine/Coroutine.h"
using namespace hiredis;
ClientOptions g_option;

const int num = 100000;
static int s, g, i, a;
class cb : public RedisAsyncCallback
{
public:
    void callback_set(int32_t ret)
    {
        s++;
        if (s == num)
            LOG_DEBUG("callback_set");
    }
    void callback_get(int32_t ret, const std::string &val)
    {
        g++;
        if (g == num)
            LOG_DEBUG("callback_get, val:" << val);
    }
    void callback_incr(int32_t ret, int64_t val)
    {
        i++;
        if (i == num)
            LOG_DEBUG("callback_incr, val:" << val);
    }
    void callback_addSetMember(int32_t ret, bool added)
    {
        a++;
        if (a == num)
            LOG_DEBUG("callback_addSetMember, added:" << added);
    }
    void callback_del(int32_t ret)
    {
        //LOG_DEBUG("callback_del");
    }
};

class TestRedis : public Application
{
public:
    void init()
    {
        auto loop = t.start();
        Co_RedisClient::getInstance().init(loop);
    }
    void loop()
    {
        usleep(1000*1000);
        CmdArg argv;
        argv << "set" << "test" << "v" + std::to_string(UtilTime::getNow());
        Coroutine::create(std::bind(&Co_RedisClient::execCommand, &Co_RedisClient::getInstance(), std::placeholders::_1), &argv);
        std::string val;
        CmdArg argv2;
        argv2 << "get" << "test";
        Coroutine::create(std::bind(&Co_RedisClient::execCommand, &Co_RedisClient::getInstance(), std::placeholders::_1), &argv2);
    }
    void destroy()
    {
    }
    EventLoopThread t;
};


int main(int argc, char *argv[])
{
    TestRedis o;
    o.main(argc, argv);
    return 0;
}