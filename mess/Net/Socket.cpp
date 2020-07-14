#include "Socket.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "Log/Logger.h"
#include <cstring>

int32_t createSocketfd()
{
    return ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
}

void setSockOpt(int32_t fd, int32_t level, int32_t optname, const void *optval, socklen_t optlen, const std::string &name)
{
    if (::setsockopt(fd, level, optname, &optval, optlen) < 0)
    {
        LOG_ERROR("setsockopt error, name:" << name);
    }
}

InetAddr::InetAddr() {}

InetAddr::InetAddr(const std::string &ip, uint16_t port)
{
    memset(&_addr, 0, sizeof(sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = ::htons(port);
    ::inet_pton(AF_INET, ip.c_str(), &_addr.sin_addr);
}

InetAddr::InetAddr(const InetAddr &oaddr) : _addr(oaddr._addr) {}

InetAddr &InetAddr::operator=(const InetAddr &oaddr)
{
    _addr = oaddr._addr;
    return *this;
}

std::string InetAddr::toString() const
{
    std::string ip = ::inet_ntoa(_addr.sin_addr);
    uint16_t port = ::ntohs(_addr.sin_port);
    char buf[32];
    sprintf(buf, "ip:%s,port:%d", ip.c_str(), port);
    return buf;
}

const sockaddr *InetAddr::getSockAddr() const
{
    return static_cast<const sockaddr *>(static_cast<const void *>(&_addr));
}

sockaddr *InetAddr::getSockAddr()
{
    return static_cast<sockaddr *>(static_cast<void *>(&_addr));
}

Socket::Socket(int32_t fd1) : _fd(fd1)
{
}

Socket::~Socket()
{
    if (_fd > 0)
        ::close(_fd);
}

bool Socket::bind(const InetAddr &addr)
{
    int32_t ret = ::bind(_fd, addr.getSockAddr(), sizeof(sockaddr_in));
    if (ret != 0)
    {
        LOG_ERROR("socket bind error");
        return false;
    }
    return true;
}

bool Socket::listen()
{
    int32_t ret = ::listen(_fd, SOMAXCONN);
    if (ret != 0)
    {
        LOG_ERROR("socket listen error");
        return false;
    }
    return true;
}

int32_t Socket::accept(InetAddr *addr)
{
    socklen_t len = sizeof(sockaddr_in);
    return ::accept4(_fd, addr->getSockAddr(), &len, SOCK_NONBLOCK | SOCK_CLOEXEC);
}

void Socket::setTcpNoDelay(bool on)
{
    int32_t opt = on ? 1 : 0;
    setSockOpt(_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof opt, "setTcpNoDelay");
}

void Socket::setReuse(bool on)
{
    int32_t opt = on ? 1 : 0;
    setSockOpt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt, "setReuseAddr");
    setSockOpt(_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof opt, "setReusePort");
}

void Socket::setKeepAlive(bool on)
{
    int32_t opt = on ? 1 : 0;
    setSockOpt(_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof opt, "setKeepAlive");
}

void Socket::shutdownWrite()
{
    ::shutdown(_fd, SHUT_WR);
}

int32_t net::connect(int32_t fd, const InetAddr &addr)
{
    return ::connect(fd, addr.getSockAddr(), sizeof(sockaddr_in));
}