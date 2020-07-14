#include "../Log/Logger.h"
#include "../Async/AsyncQueue.h"
#include <unistd.h>

class AsyncTest : public IAsyncHandler
{
public:
    virtual bool onExecute(IKernel *kernel, int32_t queueId, int32_t threadid)
    {
        LOG_DEBUG("onExecute, threadId:"<<std::this_thread::get_id());
        if(threadid % 2 == 0)
        {
            return false;
        }
        return true;
    }

    virtual bool onSuccess(IKernel *kernel)
    {
        LOG_DEBUG("onSuccess, threadId:"<<std::this_thread::get_id());
        return true;
    }

    virtual bool onFailed(IKernel *kernel)
    {
        LOG_DEBUG("onFailed, threadId:"<<std::this_thread::get_id());
        return true;
    }

    virtual bool onRelease(IKernel *kernel)
    {
        LOG_DEBUG("onRelease, threadId:"<<std::this_thread::get_id());
        return true;
    }

};


int main()
{
    LOG_MGR.ready();
    LOG_MGR.init();
    LOG_MGR.setLogInfo("./", "test");
    AsyncQueue* async = new AsyncQueue(0, "nullptr");
    async->ready();
    async->init(5);
    AsyncTest *test = new AsyncTest();
    for(int i = 0; i < 5; ++i)
    {
        async->startAsync(i, test, "test_async", __LINE__);
    }
    for(int i = 0; i < 5; ++i)
    {
        usleep(1000 * 1000);
        LOG_INFO("will start after " << 5 - i<<"s");
    }
    async->loop();
    async->destroy();
    LOG_MGR.destroy();
    return 0;
}