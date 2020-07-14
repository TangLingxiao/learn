#pragma once
#include "../Interface/IKernel.h"
#include "../Interface/ICore.h"
#include <vector>
#include <string>
class AsyncThread;
class AsyncQueue : public IAsyncQueue
{
public:
    AsyncQueue(int32_t queueId, const char *trace):_queueId(queueId), _trace(trace){}

    bool ready();
    bool init(int32_t threadNum);
    bool destroy();
    void loop();

    void startAsync(const int64_t threadid, IAsyncHandler *handler, const char *file, const int32_t line) override;
    void stopAsync(IAsyncHandler *handler) override;
    int32_t getQueueId() override;
    void getQueueInfo(int32_t &threadNum, std::set<int32_t> &threadIds) override;
private:
    int32_t _queueId{0};
    std::string _trace;
    std::vector<AsyncThread*> _threads;
};