#include "socket.h"
#include <unistd.h>
#include <memory.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "base/logmgr.h"
#include <cerrno>

int32_t createSocketfd()
{
    return ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
}

void setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen, const std::string &name)
{
    if (::setsockopt(fd, level, optname, &optval, optlen) < 0)
    {
        LOG_ERROR("setsockopt error,optname: " << name);
    }
}

InetAddr::InetAddr() : m_oAddr()
{
}

InetAddr::~InetAddr()
{
}

InetAddr::InetAddr(const std::string &strIp, uint16_t iPort) : m_oAddr()
{
    memset(&m_oAddr, 0, sizeof(sockaddr_in));
    m_oAddr.sin_family = AF_INET;
    m_oAddr.sin_port = ::htons(iPort);
    m_oAddr.sin_addr.s_addr = ::inet_addr(strIp.c_str());
}
InetAddr::InetAddr(const InetAddr &oAddr) : m_oAddr(oAddr.m_oAddr)
{
}

std::string InetAddr::toString() const
{
    std::string strIp = ::inet_ntoa(m_oAddr.sin_addr);
    uint16_t iPort = ::ntohs(m_oAddr.sin_port);
    char buf[32];
    sprintf(buf, "ip:%s, port:%d", strIp.c_str(), iPort);
    return buf;
}

const sockaddr *InetAddr::getSockaddr() const
{
    return static_cast<const sockaddr *>(static_cast<const void *>(&m_oAddr));
}

sockaddr *InetAddr::getSockaddr()
{
    return static_cast<sockaddr *>(static_cast<void *>(&m_oAddr));
}

Socket::Socket(int32_t iFd) : m_iFd(iFd)
{
}

Socket::~Socket()
{
    if (m_iFd > 0)
    {
        ::close(m_iFd);
    }
}

bool Socket::bind(const InetAddr &oAddr)
{
    int32_t iRet = ::bind(m_iFd, oAddr.getSockaddr(), static_cast<socklen_t>(sizeof(sockaddr)));
    if (iRet != 0)
    {
        LOG_ERROR("bind error");
        return false;
    }
    return true;
}

bool Socket::listen()
{
    int32_t iRet = ::listen(m_iFd, SOMAXCONN);
    if (iRet != 0)
    {
        LOG_ERROR("listen error");
        return false;
    }
    return true;
}

int32_t Socket::accept(InetAddr *pAddr)
{
    socklen_t addrlen = sizeof(sockaddr);
    return ::accept4(m_iFd, pAddr->getSockaddr(), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
}

void Socket::setTcpNoDelay(bool on)
{
    int32_t op = on ? 1 : 0;
    setsockopt(m_iFd, IPPROTO_TCP, TCP_NODELAY, &op, sizeof op, "setTcpNoDelay");
}

void Socket::setReuse(bool on)
{
    int32_t op = on ? 1 : 0;
    setsockopt(m_iFd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof op, "SetReuseAddr");
    setsockopt(m_iFd, SOL_SOCKET, SO_REUSEPORT, &op, sizeof op, "SetReusePort");
}

void Socket::setKeepAlive(bool on)
{
    int32_t op = on ? 1 : 0;
    setsockopt(m_iFd, SOL_SOCKET, SO_KEEPALIVE, &op, sizeof op, "setKeepAlive");
}

bool sockets::connect(int32_t iFd, const InetAddr &oAddr)
{
    int32_t iRet = ::connect(iFd, oAddr.getSockaddr(), sizeof(sockaddr_in));
    if (iRet != 0 && errno != EINPROGRESS)
    {
        LOG_ERROR("connect error, errno:" << errno);
        return false;
    }
    return true;
}