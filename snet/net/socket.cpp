#include "socket.h"
#include <unistd.h>
#include <netinet/in.h>
#include <memory.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "base/logmgr.h"

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
void logNewConnection(sockaddr_in *pAddr)
{
    if (pAddr == nullptr)
    {
        return;
    }
    //const sockaddr_in *pAddrIn = static_cast<const sockaddr_in *>(static_cast<const void *>(pAddr));
    std::string strIp = ::inet_ntoa(pAddr->sin_addr);
    uint16_t iPort = ::ntohs(pAddr->sin_port);
    LOG_DEBUG("new connection, ip:" << strIp << ", port:" << iPort);
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

bool Socket::bind(const std::string &strIp, uint16_t iPort)
{
    sockaddr_in oAddr;
    memset(&oAddr, 0, sizeof oAddr);
    oAddr.sin_family = AF_INET;
    oAddr.sin_port = htons(iPort);
    oAddr.sin_addr.s_addr = inet_addr(strIp.c_str());
    int32_t iRet = ::bind(m_iFd, static_cast<const sockaddr *>(static_cast<const void *>(&oAddr)), static_cast<socklen_t>(sizeof(sockaddr)));
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
int32_t Socket::accept(sockaddr_in *addr)
{
    socklen_t addrlen = sizeof(sockaddr);
    return ::accept4(m_iFd, static_cast<sockaddr*>(static_cast<void*>(addr)), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
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
