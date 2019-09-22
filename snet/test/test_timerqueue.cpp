#include "base/application.h"
#include "base/timerqueue.h"
#include "base/timeutil.h"
#include "net/eventloop.h"
#include "net/channel.h"
#include <unistd.h>
#include <string.h>
EventLoop *g_loop = nullptr;
void timeout1()
{
    auto str = TimeUtil::formatTime(TimeUtil::getNow());
    printf("timeout1 %s, %ld\n", str.c_str(), TimeUtil::getNowMs());
    //    g_loop->quit();
}

void timeout2()
{
    auto str = TimeUtil::formatTime(TimeUtil::getNow());
    printf("timeout2 %s, %ld\n", str.c_str(), TimeUtil::getNowMs());
    //    g_loop->quit();
}
void timeout3()
{
    auto str = TimeUtil::formatTime(TimeUtil::getNow());
    printf("timeout3 %s, %ld\n", str.c_str(), TimeUtil::getNowMs());
    //    g_loop->quit();
}
void timeout4()
{
    auto str = TimeUtil::formatTime(TimeUtil::getNow());
    printf("timeout4 %s, %ld\n", str.c_str(), TimeUtil::getNowMs());
    g_loop->quit();
}
class Test : public App
{
public:
    void loop()
    {
        EventLoop loop;
        g_loop = &loop;
        g_loop->runAfter(1, std::bind(&timeout1));
        g_loop->runAfter(2.5, std::bind(&timeout2));
        g_loop->runAfter(2, std::bind(&timeout3), true);
        g_loop->runAfter(10, std::bind(&timeout4));
        g_loop->loop();
        g_loop->quit();
    }
};

int main(int argc, char **argv)
{
    Test g_test;
    g_test.main(argc, argv);
}