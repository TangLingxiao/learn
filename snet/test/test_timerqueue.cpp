#include "base/application.h"
#include "base/timerqueue.h"
#include "base/timeutil.h"
#include "net/eventloop.h"
#include "net/channel.h"
#include <unistd.h>
#include <string.h>
EventLoop *g_loop = nullptr;
void timeout()
{
    printf("Timeout\n");
    g_loop->quit();
}

class Test : public App
{
public:
    void loop()
    {
        EventLoop loop;
        g_loop = &loop;
        g_loop->runAfter(10000, std::bind(&timeout));
        auto iBegin = TimeUtil::getNowMs();
        g_loop->loop();
        auto iEnd = TimeUtil::getNowMs();
        printf("timeout: diff %ld\n", iEnd - iBegin);
    }
};

int main(int argc, char **argv)
{
    Test g_test;
    g_test.main(argc, argv);
}