#pragma once

#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <stdint.h>

int32_t createSocketfd();

class InetAddr
{
public:
    InetAddr();
    InetAddr(const std::string &ip, uint16_t port);
    InetAddr(const InetAddr &oaddr);
    InetAddr &operator=(const InetAddr &oaddr);
    ~InetAddr() = default;

    std::string toString() const;
    const sockaddr *getSockAddr() const;
    sockaddr *getSockAddr();

private:
    sockaddr_in _addr;
};

class Socket
{
public:
    Socket(int32_t fd1);
    ~Socket();
    bool bind(const InetAddr &addr);
    bool listen();
    int32_t accept(InetAddr *addr);
    void setTcpNoDelay(bool on);
    void setReuse(bool on);
    void setKeepAlive(bool on);
    int32_t fd() { return _fd; }
    void shutdownWrite();
private:
    int32_t _fd{-1};
};

namespace net
{
    int32_t connect(int32_t fd, const InetAddr &addr);
}
