#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <sys/socket.h>
#include "base/noncopyable.h"
#include <string>
#include <memory>
#include <netinet/in.h>

int32_t createSocketfd();

class InetAddr
{
public:
    InetAddr();
    InetAddr(const std::string &strIp, uint16_t iPort);
    ~InetAddr();
    std::string toString() const;
    const sockaddr *getSockaddr() const;
    sockaddr *getSockaddr();

private:
    sockaddr_in m_oAddr;
};

class Socket : public NonCopyable
{
public:
    Socket(int32_t iFd);
    ~Socket();
    bool bind(const InetAddr &oAddr);
    bool listen();
    int32_t accept(InetAddr *pAddr);
    bool connect(const InetAddr &oAddr);
    void setTcpNoDelay(bool on);
    void setReuse(bool on);
    void setKeepAlive(bool on);
    int32_t fd() { return m_iFd; }

private:
    int32_t m_iFd;
};
#endif