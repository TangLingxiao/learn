#include "net/connector.h"
#include "base/logmgr.h"
#include "net/eventloop.h"
#include "base/application.h"
#include "net/socket.h"
void newConnectionCb(EventLoop *loop)
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
        Connector co(&t);
        co.setNewConnectionCb(std::bind(&newConnectionCb, &t));
        co.start("172.19.130.180", 6666);
        t.loop();
    }
};
int main()
{
    Test test;
    test.main(0, nullptr);
    return 0;
}
