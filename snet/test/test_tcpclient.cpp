#include "net/tcpclient.h"
#include "base/logmgr.h"
#include "net/eventloopthread.h"
#include "base/application.h"
#include "net/socket.h"
#include "net/tcpconnection.h"
#include "net/eventloop.h"
void onMessage(const TcpConnectionPtr &pConn, const std::string &strMsg, TcpClient * pClient, EventLoop *loop)
{
    LOG_DEBUG("onMessage:" << strMsg);
    loop->quit();
}

class Test : public App
{
public:
    void onConnected(const TcpConnectionPtr &pConn, const std::string &strMsg)
    {
        pConn->send(strMsg);
    }
    void loop() override
    {
        EventLoop t;
        TcpClient client(&t);
        client.setMsgCallBack(std::bind(&onMessage, std::placeholders::_1, std::placeholders::_2, &client, &t));

        client.setConnectedCallBack(std::bind(&Test::onConnected, this, std::placeholders::_1, "hello"));
        client.start("172.19.130.180", 6666);
        t.loop();
    }
};
int main()
{
    Test test;
    test.main(0, nullptr);
    return 0;
}
