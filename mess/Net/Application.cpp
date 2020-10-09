#include "Application.h"
#include "TcpClient.h"
#include "TcpServer.h"
#include "Log/Logger.h"
#include <signal.h>
#include <unistd.h>
#include "EventLoopThread.h"
#include <chrono>

std::shared_ptr<TcpServer> Application::_server;
std::shared_ptr<TcpClient> Application::_client;

Application::~Application()
{
    terminate();
}

void Application::terminate()
{
    std::lock_guard<std::mutex> lock(_mtx);
    _terminate = true;
    _cond.notify_one();
}

void Application::loop()
{
}

int Application::main(int argc, char *argv[])
{
    try
    {
        LOG_MGR.ready();
        LOG_MGR.init();
        //LOG_MGR.setLogLevel(LEVEL_INFO);
        LOG_MGR.setLogInfo("./", "test");
        EventLoopThread serverThread, clientThread;
        auto elServer = serverThread.start();
        auto elClient = clientThread.start();
        _server.reset(new TcpServer(elServer, "server", "127.0.0.1", 8888));
        _client.reset(new TcpClient(elClient, "client"));
        init();
        _server->start();
        while (!_terminate)
        {
            std::unique_lock<std::mutex> lock(_mtx);
            _cond.wait_for(lock, std::chrono::milliseconds() * 100);
            loop();
        }
        destroy();
        LOG_MGR.destroy();
    }
    catch (std::exception &e)
    {
        LOG_ERROR("application exception: " << e.what());
        return -1;
    }
    return 0;
}