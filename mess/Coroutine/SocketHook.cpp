#include "Net/Socket.h"
#include "Net/EventLoop.h"
#include "Net/Channel.h"
#include "Coroutine.h"
#include "Timer/TimerMgr.h"
#include "HookApi.h"
#include "CoSocket.h"
#include <unistd.h>
extern EventLoop *g_loop;
int coroutine_socket(int domain, int type, int protocol)
{
    int32_t fd = ::socket(domain, type | SOCK_NONBLOCK | SOCK_CLOEXEC, protocol);
    s_mSockets.emplace(fd, std::unique_ptr<CoSocket>(new CoSocket(g_loop, fd)));
    return fd;
}

ssize_t coroutine_send(int sockfd, const void *buf, size_t len, int flags)
{
    auto it = s_mSockets.find(sockfd);
    if (it == s_mSockets.end())
    {
        return ::send(sockfd, buf, len, flags);
    }
    return it->second->coSend(buf, len, flags);
}

ssize_t coroutine_read(int fd, void *buf, size_t count)
{
    //非sockfd的处理todo
    auto it = s_mSockets.find(fd);
    if (it == s_mSockets.end())
    {
        return ::read(fd, buf, count);
    }
    return it->second->coRead(buf, count);
}
ssize_t coroutine_write(int fd, const void *buf, size_t count)
{
    //非sockfd的处理todo
    auto it = s_mSockets.find(fd);
    if (it == s_mSockets.end())
    {
        return ::write(fd, buf, count);
    }
    return it->second->coWrite(buf, count);
}

ssize_t coroutine_recv(int sockfd, void *buf, size_t len, int flags)
{
    auto it = s_mSockets.find(sockfd);
    if (it == s_mSockets.end())
    {
        return ::recv(sockfd, buf, len, flags);
    }
    return it->second->coRecv(buf, len, flags);
}

int coroutine_close(int fd)
{
    auto it = s_mSockets.find(fd);
    if (it == s_mSockets.end())
    {
        return ::close(fd);
    }
    s_mSockets.erase(fd);
    return 0;
}

int coroutine_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    auto it = s_mSockets.find(sockfd);
    if (it == s_mSockets.end())
    {
        return ::connect(sockfd, addr, addrlen);
    }
    return it->second->coConnect(addr, addrlen);
}

int coroutine_poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    Coroutine *co = Coroutine::getCurrentSafe();
    if (!co)
    {
        return ::poll(fds, nfds, timeout);
    }
    g_loop->runAfter(timeout, [&co]() {
        co->resume();
    });
    co->yield();
    return ::poll(fds, nfds, 0);
}

ssize_t coroutine_sendmsg(int sockfd, const struct msghdr *msg, int flags)
{
    auto it = s_mSockets.find(sockfd);
    if (it == s_mSockets.end())
    {
        return ::sendmsg(sockfd, msg, flags);
    }
    return it->second->coSendmsg(msg, flags);
}
ssize_t coroutine_recvmsg(int sockfd, struct msghdr *msg, int flags)
{
    auto it = s_mSockets.find(sockfd);
    if (it == s_mSockets.end())
    {
        return ::recvmsg(sockfd, msg, flags);
    }
    return it->second->coRecvmsg(msg, flags);
}
int coroutine_getaddrinfo(const char *name,
                          const char *service,
                          const struct addrinfo *req,
                          struct addrinfo **pai)
{
    return ::getaddrinfo(name, service, req, pai);
}

struct hostent *coroutine_gethostbyname(const char *name)
{
    return ::gethostbyname(name);
}