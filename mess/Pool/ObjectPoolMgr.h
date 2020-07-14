#ifndef __OBJECT_POOL_MANAGER_H__
#define __OBJECT_POOL_MANAGER_H__

#include <map>
#include "Pattern/Singleton.h"

class ObjectPoolBase;
class ObjectPoolManager : public Singleton<ObjectPoolManager>
{
public:
    ObjectPoolManager();
    ~ObjectPoolManager();

public:
    int32_t init();
    void onServerTerminate();

    void recycle(const char* pObjectName, void* pData, const char* pFileName, int32_t iLine, uint64_t iHashCode);
    void Register(uint64_t iHashCode, ObjectPoolBase* pPool);
    void dump(uint32_t iForce = 0);

private:
    int64_t m_iLastPrintTime;
    std::map<uint64_t, ObjectPoolBase*> m_mapObjectPool;
};

#endif
