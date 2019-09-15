#include "base/application.h"
#include "net/eventloop.h"
#include "net/channel.h"
#include <sys/timerfd.h>
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
        int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
        Channel channel(g_loop, timerfd);
        channel.setReadCallback(timeout);
        channel.enableReading();

        struct itimerspec howlong;
        bzero(&howlong, sizeof howlong);
        howlong.it_value.tv_sec = 5;
        ::timerfd_settime(timerfd, 0, &howlong, nullptr);
        g_loop->loop();
        ::close(timerfd);
    }
};

int main(int argc, char **argv)
{
    Test g_test;
    g_test.main(argc, argv);
}