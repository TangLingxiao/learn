#pragma once

#include <memory>
#include <mutex>
#include <condition_variable>

class TcpClient;
class TcpServer;
class Application
{
public:
    virtual ~Application();
    int main(int argc, char *argv[]);
    void terminate();
protected:
    virtual void init() = 0;
    virtual void destroy() = 0;
    virtual void loop();
private:
    static std::shared_ptr<TcpServer> _server;
    static std::shared_ptr<TcpClient> _client;
    std::mutex _mtx;
    std::condition_variable _cond;
    bool _terminate{false};
};