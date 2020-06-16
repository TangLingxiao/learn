#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <algorithm>

int main(int argc, char **argv)
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(sockaddr));
    addr.sin_family = AF_INET;
    // if(argc != 3)
    // {
    //     printf("usage: ip port\n");
    // }
    // addr.sin_port = htons(atoi(argv[2]));
    // ::inet_pton(AF_INET, argv[1], &addr.sin_addr);
    addr.sin_port = htons(6666);
    ::inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    again:int ret = ::connect(fd, (sockaddr*)&addr, sizeof(addr));
    if(ret != 0)
    {
        goto again;
    }
    char buf[65535];
    fd_set redset;
    FD_ZERO(&redset);
    //阻塞
    // while(fgets(buf, sizeof(buf),stdin))
    // {
    //     ::write(fd,buf, strlen(buf));
    //     ::read(fd, buf, sizeof(buf));    
    //     fputs(buf,stdout);
    // }
    
    //select
    bool eof = false;
    while(1)
    {
        if(!eof)
            FD_SET(fileno(stdin), &redset);
        FD_SET(fd, &redset);
        int maxfd1 = std::max(fileno(stdin),fd)+1;
        int n = ::select(maxfd1,&redset, nullptr, nullptr, nullptr);
        if(n < 0)
            break;
        if(n == 0)
            continue;
        if(FD_ISSET(fileno(stdin), &redset))
        {
            n = ::read(fileno(stdin), buf, sizeof(buf));
            if (n == 0)
            {
                eof = true;
                shutdown(fd, SHUT_WR);
                FD_CLR(fileno(stdin),&redset);
                continue;
            }
            ::write(fd, buf, strlen(buf));
            printf("%s\n",buf);
        }
        if(FD_ISSET(fd, &redset))
        {
            if(n = ::read(fd, buf, sizeof(buf)) == 0)
            {
                if(eof)
                    return 0;
                return 0;
            }
            //fputs(buf, stdout);
        }
    }
    return 0;
}
