#include "base/application.h"
#include "net/eventloopthread.h"
#include "net/eventloop.h"
#include <cassert>
#include <stdio.h>
#include "base/timerqueue.h"
#include <unistd.h>

void print(EventLoop *pLoop)
{
    printf("runafter\n");
    pLoop->quit();
}

class Test : public App
{
public:
    void loop() override
    {
        EventLoopThread t;
        auto pLoop = t.getEventLoop();
        assert(pLoop != nullptr);
        pLoop->runAfter(5.5, std::bind(print, pLoop), false);
        usleep(5.5*1000*1000);
    }
};
int main()
{
    Test test;
    test.main(0, nullptr);
    return 0;
}
