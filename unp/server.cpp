#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>
#include <set>
#include <netinet/tcp.h>
#include <poll.h>
#include <vector>
void sig_child(int no)
{
    pid_t pid;
    int stat;
    while (pid = waitpid(-1, &stat, WNOHANG) > 0)
    {
        printf("child %d terminated\n", pid);
    }
    return;
}

int main()
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    int op = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof op);
    setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &op, sizeof op);
    sockaddr_in srvaddr;
    memset(&srvaddr, 0, sizeof(sockaddr_in));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(6666);
    ::inet_pton(AF_INET, "127.0.0.1", &srvaddr.sin_addr);
    ::bind(fd, (sockaddr *)&srvaddr, sizeof(sockaddr_in));
    ::listen(fd, 1000);
    signal(SIGCHLD, sig_child);
    std::vector<pollfd> vfds;
    pollfd listen;
    listen.fd = fd;
    listen.events |= POLLIN;
    vfds.push_back(listen);
    for (;;)
    {
        int n = ::poll(&*vfds.begin(), vfds.size(), 10000);
        if (n < 0)
        {
            exit(-1);
        }
        if (n == 0)
        {
            continue;
        }
        std::vector<pollfd> active;
        for (auto &rfd : vfds)
        {
            if (rfd.revents > 0)
            {
                active.push_back(rfd);
            }
        }
        for (auto it = active.begin(); it != active.end();)
        {
            if (it->revents & POLLIN)
            {
                if (it->fd == fd)
                {
                    int connfd = ::accept(fd, nullptr, nullptr);
                    if (connfd < 0)
                    {
                        if (connfd == EINTR)
                        {
                            ++it;
                            continue;
                        }
                        else
                            exit(-1);
                    }
                    pollfd newfd;
                    newfd.fd = connfd;
                    newfd.events |= POLLIN;
                    vfds.push_back(newfd);
                }
                else
                {
                    char buf[65535];
                    int len = ::read(it->fd, buf, sizeof(buf));
                    if (len <= 0)
                    {
                        it = active.erase(it);
                        ::close(it->fd);
                        continue;
                    }
                    else
                    {
                        int n = ::write(it->fd, buf, strlen(buf));
                        printf("n: %d, len: %d, buf: %s\n", n, len, buf);
                    }
                }
            }
            ++it;
        }
    }
    //select version
    // fd_set allset, rset;
    // FD_ZERO(&allset);
    // FD_ZERO(&rset);
    // FD_SET(fd, &allset);
    // std::set<int> sfd;
    // int maxfd1 = fd;
    // while(1)
    // {
    //     rset = allset;
    //     int n = ::select(maxfd1 + 1, &rset, nullptr, nullptr, 0);
    //     if(FD_ISSET(fd, &rset))
    //     {
    //         int connfd = ::accept(fd, nullptr, nullptr);
    //         if (connfd < 0)
    //         {
    //             if (errno == EINTR)
    //                 continue;
    //             else
    //             {
    //                 printf("accept error");
    //                 break;
    //             }
    //         }
    //         FD_SET(connfd, &allset);
    //         sfd.insert(connfd);
    //         maxfd1 = connfd > maxfd1?connfd:maxfd1;
    //     }
    //     for(auto rfd :sfd)
    //     {
    //         if(FD_ISSET(rfd, &rset))
    //         {
    //             char buf[65535];
    //             if(int len = ::read(rfd, buf, sizeof(buf))==0)
    //             {
    //                 FD_CLR(rfd, &allset);
    //                 ::close(rfd);
    //             }
    //             else
    //             {
    //                 int n = ::write(rfd, buf, strlen(buf));
    //                 printf("n: %d, len: %d, buf: %s\n", n, strlen(buf), buf);
    //             }

    //         }
    //     }
    // }
    return 0;
}