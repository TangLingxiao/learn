#include "../Log/Logger.h"
#include <unistd.h>
#include "TcpServer.h"
#include "Net/EventLoop.h"
#include "Net/TcpConnection.h"

void msg(const TcpConnectionPtr &conn, const std::string &msg)
{
    LOG_INFO("MSG" << msg);
    conn->send(msg);
}

void conn(const TcpConnectionPtr &conn)
{
    LOG_INFO("CONN, peer:" << conn->getPeer());
}

int main()
{
    LOG_MGR.ready();
    LOG_MGR.init();
    LOG_MGR.setLogLevel(LEVEL_INFO);
    LOG_MGR.setLogInfo("./", "test");
    EventLoop loop;
    TcpServer server(&loop, "test", "127.0.0.1", 5444);
    server.setThreadNum(5);
    server.setMsgCb(std::bind(&msg, std::placeholders::_1, std::placeholders::_2));
    server.setConnCb(std::bind(&conn, std::placeholders::_1));
    server.start();
    loop.loop();
    LOG_MGR.destroy();
    return 0;
}