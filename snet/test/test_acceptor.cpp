#include "net/acceptor.h"
#include "base/logmgr.h"
#include "net/eventloop.h"
#include "base/application.h"
#include <netinet/in.h>

void newConnectionCb(int32_t iFd, sockaddr_in *addr, EventLoop *loop)
{
    LOG_INFO("==================newconnection===============");
    //loop->quit();
}
class Test : public App
{
public:
    void loop() override
    {
        EventLoop t;
        Acceptor ap(&t, "172.19.130.180", 6666);
        ap.setNewConnectionCb(std::bind(&newConnectionCb, std::placeholders::_1, std::placeholders::_2, &t));
        t.runInLoop(std::bind(&Acceptor::listen, &ap));
        t.loop();
    }
};
int main()
{
    Test test;
    test.main(0, nullptr);
    return 0;
}
