#include "Log/Logger.h"
#include <unistd.h>
#include "Net/EventLoopThread.h"
#include "Coroutine/Coroutine.h"
#include "Coroutine/SocketHook.h"
#include "Coroutine/CoSocket.h"
#include "arpa/inet.h"
#include "DB/Redis/RedisClient.h"

extern EventLoop *g_loop;
void func(void *)
{
    auto fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ::inet_addr("127.0.0.1");
    addr.sin_port = ::htons(5444);
    connect(fd, (sockaddr *)&addr, sizeof addr);
    std::string hello = "hello";
    auto wrote = write(fd, hello.c_str(), hello.size());
    LOG_DEBUG("wrote size:" << wrote);
    char buf[50];
    auto read = read(fd, buf, 50);
    hello.assign(buf, read);
    LOG_DEBUG("read:" << hello);
    close(fd);
}

void redis(void *)
{
    auto client = hiredis::CLIENT_MGR.getRedisClient();
    std::string key = "test";
    client->setString(key, "testval");
    LOG_DEBUG("set KEY:" << key);
    std::string val;
    client->getString(key, val);
    LOG_DEBUG("get KEY:" << key << " , val:" << val);
    Coroutine::create(&func, nullptr);
}

int main()
{
    LOG_MGR.ready();
    LOG_MGR.init();
    LOG_MGR.setLogLevel(LEVEL_DEBUG);
    LOG_MGR.setLogInfo("./", "test");
    EventLoopThread lp;
    g_loop = lp.start();
    // Coroutine::create(&func, nullptr);
    // Coroutine::create(&func, nullptr);
    // Coroutine::create(&func, nullptr);
    // Coroutine::create(&func, nullptr);
    // Coroutine::create(&func, nullptr);
    hiredis::Client *client;
    Coroutine::create([&client](void *) {
        hiredis::ClientOptions stClientConf;
        stClientConf.m_sHost = "127.0.0.1";
        hiredis::CLIENT_MGR.init(stClientConf);
    },
                      nullptr);
    for (int i = 0; i < 5; ++i)
    {
        Coroutine::create(&redis, nullptr);
    }

    usleep(1 * 1000 * 1000);

    LOG_MGR.destroy();
    return 0;
}