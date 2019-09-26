#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <sys/socket.h>
#include "base/noncopyable.h"
#include <string>

struct sockaddr;

int32_t createSocketfd();
void logNewConnection(sockaddr *pAddr);

class Socket : public NonCopyable
{
public:
    Socket(int32_t iFd);
    ~Socket();
    bool bind(const std::string &strIp, uint16_t iPort);
    bool listen();
    int32_t accept(sockaddr *addr, socklen_t *addrlen);
    void setTcpNoDelay(bool on);
    void setReuse(bool on);
    void setKeepAlive(bool on);
    int32_t fd() { return m_iFd; }

private:
    int32_t m_iFd;
};
#endif