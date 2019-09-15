#include "net/eventloop.h"
#include "base/logmgr.h"

int main()
{
    if (!LogMgr::getInstance().init("algo.log", "./", true))
    {
        exit(-1);
    }
    LOG_DEBUG("main thread " << std::this_thread::get_id());
    EventLoop ev,et;
    ev.loop();
    et.loop();
    
    LogMgr::getInstance().release();
    exit(0);
    return 0;
}