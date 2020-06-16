#include "../Log/Logger.h"

int main()
{
    LOG_MGR.ready();
    LOG_MGR.init();
    LOG_MGR.setLogInfo("./", "test");
    for(int i = 0; i<10; i++)
    {
        LOG_DEBUG("test debug i:" << i);
        LOG_INFO("test info i:" << i);
        LOG_WARN("test warn i:" << i);
        LOG_ERROR("test error i:" << i);
    }
    LOG_MGR.destroy();
    return 0;
}