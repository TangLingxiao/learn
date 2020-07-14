#include "Log/Logger.h"
#include <unistd.h>
#include "TcpClient.h"
#include "Net/EventLoop.h"
#include "Net/TcpConnection.h"

void msg(const TcpConnectionPtr &conn, const std::string &msg)
{
    LOG_INFO("MSG" << msg);
    conn->shutdown();
}

void conn(const TcpConnectionPtr &conn)
{
    LOG_INFO("CONN, peer:" << conn->getPeer());
    conn->send("test");
}

int main()
{
    LOG_MGR.ready();
    LOG_MGR.init();
    LOG_MGR.setLogLevel(LEVEL_INFO);
    LOG_MGR.setLogInfo("./", "test");
    EventLoop loop;
    TcpClient client(&loop, "test");
    client.setMsgCb(std::bind(&msg, std::placeholders::_1, std::placeholders::_2));
    client.setConnCb(std::bind(&conn, std::placeholders::_1));
    client.connect("127.0.0.1", 5444);
    loop.loop();
    LOG_MGR.destroy();
    return 0;
}