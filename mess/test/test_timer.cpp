#include "../Log/Logger.h"
#include "Timer/TimerMgr.h"
#include <unistd.h>

int main()
{
    LOG_MGR.ready();
    LOG_MGR.init();
    LOG_MGR.setLogInfo("./", "test");
    TimerMgr::getInstance().init();
    TimerMgr::getInstance().setTimer(2000, 5, [](){
        LOG_DEBUG("Timer1");
    });
    TimerMgr::getInstance().setTimer(5000, 2, [](){
        LOG_DEBUG("Timer2");
    });
    TimerMgr::getInstance().setTimer(10000, 1, [](){
        LOG_DEBUG("Timer3");
    });
    int i = 0;
    while(i++<11)
    {
        TimerMgr::getInstance().update(0);
        usleep(1000 * 1000);
    }
    TimerMgr::getInstance().destroy();
    LOG_MGR.destroy();
    return 0;
}