#include "net/tcpserver.h"
#include "base/logmgr.h"
#include "net/eventloop.h"
#include "base/application.h"
#include "net/socket.h"
#include "net/tcpconnection.h"

void onMessage(const TcpConnectionPtr &pConn, const std::string &strMsg)
{
    LOG_DEBUG("onMessage:" << strMsg);
    std::string strOut;
    pConn->packMsg(strMsg, strOut);
    pConn->send(strOut);
}

class Test : public App
{
public:
    void loop() override
    {
        EventLoop t;
        TcpServer server(&t, "172.19.130.180", 6666, "Test");
        server.setMsgCallBack(std::bind(&onMessage, std::placeholders::_1, std::placeholders::_2));
        server.start();
        t.loop();
    }
};
int main()
{
    Test test;
    test.main(0, nullptr);
    return 0;
}
