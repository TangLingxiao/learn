#include "base/application.h"
#include "net/eventloopthreadpool.h"
#include "net/eventloop.h"
#include <cassert>
#include <stdio.h>
#include "base/timerqueue.h"
#include <unistd.h>

void print(EventLoop *pLoop)
{
    printf("runafter,%p\n", pLoop);
    pLoop->quit();
}

class Test : public App
{
public:
    void loop() override
    {
        EventLoop loop;
        EventLoopThreadPool pool(5, &loop);
        pool.start();
        auto pLoop1 = pool.getNextLoop();
        assert(pLoop1 != nullptr);
        pLoop1->runAfter(5.5, std::bind(print, pLoop1), false);
        LOG_DEBUG("pLoop1========================== " << pLoop1);
        auto pLoop2 = pool.getNextLoop();
        assert(pLoop2 != nullptr);
        pLoop2->runAfter(5.5, std::bind(print, pLoop2), false);
        LOG_DEBUG("pLoop2========================== " << pLoop2);
        auto pLoop3 = pool.getNextLoop();
        assert(pLoop3 != nullptr);
        pLoop3->runAfter(5.5, std::bind(print, pLoop3), false);
        LOG_DEBUG("pLoop3========================== " << pLoop3);
        auto pLoop4 = pool.getNextLoop();
        assert(pLoop4 != nullptr);
        pLoop4->runAfter(5.5, std::bind(print, pLoop4), false);
        LOG_DEBUG("pLoop4========================== " << pLoop4);
        auto pLoop5 = pool.getNextLoop();
        assert(pLoop5 != nullptr);
        pLoop5->runAfter(5.5, std::bind(print, pLoop5), false);
        LOG_DEBUG("pLoop5========================== " << pLoop5);
        auto pLoop6 = pool.getNextLoop();
        assert(pLoop6 != nullptr);
        pLoop6->runAfter(5.5, std::bind(print, pLoop6), false);
        LOG_DEBUG("pLoop6========================== " << pLoop6);
        usleep(5.5 * 1000 * 1000);
    }
};
int main()
{
    Test test;
    test.main(0, nullptr);
    return 0;
}
