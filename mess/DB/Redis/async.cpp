#include "client.h"
#include <cstring>
#include <cassert>
#include "reply.h"

using namespace hiredis;

int Client::async_setString(const RedisAsyncCbPtr &cb, const std::string &key, const std::string &val)
{
    CLIENT_MGR.startAsync(cb, __FILE__, __LINE__, "SET", key, val);
    return 0;
}
int Client::async_getString(const RedisAsyncCbPtr &cb, const std::string &key)
{
    CLIENT_MGR.startAsync(cb, __FILE__, __LINE__, "GET", key);
    return 0;
}
// int Client::async_getStringBatch(const std::vector<std::string> &vKey, std::map<std::string, std::string> &mKeyValue)
// {
// }
// int Client::async_setStringEx(const std::string &key, const std::string &val, uint expire)
// {
// }
// int Client::async_setStringWithOption(const std::string &key, const std::string &val, SetCmdOption iOption, uint32_t iExpireMS, bool &bSuccess)
// {
// }
