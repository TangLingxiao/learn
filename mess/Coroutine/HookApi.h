#pragma once
#ifdef __cplusplus
extern "C"{
#endif

#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/statvfs.h>
#include <stdint.h>
#include <netdb.h>
#include <poll.h>
#include <dirent.h>

/**
 * base
 */
uint8_t coroutine_is_in();
long coroutine_get_current_id();
void coroutine_sleep(int sec);
void coroutine_usleep(int usec);
/**
 * file
 */
int coroutine_access(const char *pathname, int mode);
int coroutine_open(const char *pathname, int flags, mode_t mode);
ssize_t coroutine_read(int fd, void *buf, size_t count);
ssize_t coroutine_write(int fd, const void *buf, size_t count);
off_t coroutine_lseek(int fd, off_t offset, int whence);
int coroutine_fstat(int fd, struct stat *statbuf);
int coroutine_readlink(const char *pathname, char *buf, size_t len);
int coroutine_unlink(const char *pathname);
int coroutine_mkdir(const char *pathname, mode_t mode);
int coroutine_rmdir(const char *pathname);
int coroutine_rename(const char *oldpath, const char *newpath);
int coroutine_flock(int fd, int operation);
int coroutine_flock_ex(char *filename, int fd, int operation);
int coroutine_statvfs(const char *path, struct statvfs *buf);
/**
 * dir
 */
DIR *coroutine_opendir(const char *name);
struct dirent *coroutine_readdir(DIR *dirp);
int coroutine_closedir(DIR *dirp);
/**
 * socket
 */
int coroutine_socket(int domain, int type, int protocol);
ssize_t coroutine_send(int sockfd, const void *buf, size_t len, int flags);
ssize_t coroutine_sendmsg(int sockfd, const struct msghdr *msg, int flags);
ssize_t coroutine_recv(int sockfd, void *buf, size_t len, int flags);
ssize_t coroutine_recvmsg(int sockfd, struct msghdr *msg, int flags);
int coroutine_close(int fd);
int coroutine_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int coroutine_poll(struct pollfd *fds, nfds_t nfds, int timeout);
int coroutine_socket_set_timeout(int fd, int which, double timeout);
int coroutine_socket_wait_event(int fd, int event, double timeout);
int coroutine_getaddrinfo(const char *name,
                                 const char *service,
                                 const struct addrinfo *req,
                                 struct addrinfo **pai);
struct hostent *coroutine_gethostbyname(const char *name);
/**
 * wait
 */
size_t coroutine_wait_count();
pid_t coroutine_waitpid(pid_t __pid, int *__stat_loc, int __options);
pid_t coroutine_wait(int *__stat_loc);

#ifdef __cplusplus
}
#endif