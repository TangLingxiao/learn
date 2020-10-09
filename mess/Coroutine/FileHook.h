#pragma once
#include "HookApi.h"
#define access(pathname, mode) coroutine_access(pathname, mode)
#define open(pathname, flags, mode) coroutine_open(pathname, flags, mode)
#define read(fd, buf, count) coroutine_read(fd, buf, count)
#define write(fd, buf, count) coroutine_write(fd, buf, count)
#define lseek(fd, offset, whence) coroutine_lseek(fd, offset, whence)
#define fstat(fd, statbuf) coroutine_fstat(fd, statbuf)
#define readlink(fd, buf, size) coroutine_readlink(fd, buf, size)
#define unlink(pathname) coroutine_unlink(pathname)
#define mkdir(pathname, mode) coroutine_mkdir(pathname, mode)
#define rmdir(pathname) coroutine_rmdir(pathname)
#define rename(oldpath, newpath) coroutine_rename(oldpath, newpath)

#define opendir(name) coroutine_opendir(name)
#define readdir(dir) coroutine_readdir(dir)
#define closedir(dir) coroutine_closedir(dir)
