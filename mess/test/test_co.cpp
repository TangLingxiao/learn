
#include "Net/Application.h"
#include "Log/Logger.h"
#include "Util/TimeHelper.h"
#include "3rdparty/include/libco/co_routine.h"
#include <unistd.h>
#include "DB/Redis/RedisClient.h"
#include "Coroutine/Coroutine.h"
struct A
{
    int n;
};
void *foo(void *arg)
{
    int i{0};
    while (i++ < 5)
    {
        LOG_DEBUG("xxxxxxx, n = " << ((A *)arg)->n * i);
        co_yield_ct();
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    LOG_MGR.ready();
    LOG_MGR.init();
    LOG_MGR.setLogInfo("./", "test");
    long _cid;
    long cid = Coroutine::create(
        [](void *arg) {
            long cid = Coroutine::getCurrentCid();
            Coroutine *co = Coroutine::getByCid(cid);
            co->yield();
            *(long *) arg = Coroutine::getCurrentCid();
        },
        &_cid);

    
    Coroutine::getByCid(cid)->resume();
    LOG_MGR.destroy();
    return 0;
}