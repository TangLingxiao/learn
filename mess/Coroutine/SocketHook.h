#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <poll.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "HookApi.h"

#define socket(domain, type, protocol) coroutine_socket(domain, type, protocol)
#define send(sockfd, buf, len, flags) coroutine_send(sockfd, buf, len, flags)
#define read(sockfd, buf, len) coroutine_read(sockfd, buf, len)
#define write(sockfd, buf, len) coroutine_write(sockfd, buf, len)
#define recv(sockfd, buf, len, flags) coroutine_recv(sockfd, buf, len, flags)
#define close(fd) coroutine_close(fd)
#define connect(sockfd, addr, addrlen) coroutine_connect(sockfd, addr, addrlen)
#define poll(fds, nfds, timeout) coroutine_poll(fds, nfds, timeout)
#define sendmsg(sockfd, msg, flags) coroutine_sendmsg(sockfd, msg, flags)
#define recvmsg(sockfd, msg, flags) coroutine_recvmsg(sockfd, msg, flags)
#define getaddrinfo(name, service, req, pai) coroutine_getaddrinfo(name, service, req, pai)
#define gethostbyname(name) coroutine_gethostbyname(name)

#ifdef __cplusplus
}

#endif