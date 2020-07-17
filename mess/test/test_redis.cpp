#include "../Log/Logger.h"
#include "DB/Redis/client.h"
#include <unistd.h>

using namespace hiredis;
ClientOptions g_option;

class cb : public RedisAsyncCallback
{
public:
    void onCall(int32_t ret, ReplyPtr &reply)
    {
        std::string val = parser<std::string>(reply);
        LOG_DEBUG("GETSTRING "<<val);
    }
};
int main()
{
    LOG_MGR.ready();
    LOG_MGR.init();
    LOG_MGR.setLogInfo("./", "test");
    g_option.m_sHost = "127.0.0.1";
    CLIENT_MGR.init(g_option);
    
    auto client = CLIENT_MGR.getRedisClient();
    
    //async
    std::shared_ptr<RedisAsyncCallback> getCb(new cb);
    client->async_getString(getCb,"wsr");
    int i = 0;
    while(i++ < 10)
    {
        usleep(1000 * 1000);
        CLIENT_MGR.loop();
    }
    LOG_MGR.destroy();
    return 0;
}