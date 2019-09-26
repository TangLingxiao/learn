#include "net/accepter.h"
#include "base/logmgr.h"
#include "net/eventloop.h"
#include "base/application.h"
#include <sys/socket.h>

void newConnectionCb(int32_t iFd, sockaddr *addr, EventLoop *loop)
{
    LOG_INFO("==================newconnection===============");
    loop->quit();
}
class Test : public App
{
public:
    void loop() override
    {
        EventLoop t;
        Accepter ap(&t, "0.0.0.0", 6666);
        ap.setNewConnectionCb(std::bind(&newConnectionCb, std::placeholders::_1, std::placeholders::_2, &t));
        t.runInLoop(std::bind(&Accepter::listen, &ap));
        t.loop();
    }
};
int main()
{
    Test test;
    test.main(0, nullptr);
    return 0;
}
