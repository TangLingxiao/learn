#include "ObjectPoolMgr.h"
#include "ObjectPool.h"


ObjectPoolManager::ObjectPoolManager()
{
}

ObjectPoolManager::~ObjectPoolManager()
{
}

int32_t ObjectPoolManager::init()
{
    m_iLastPrintTime = UtilTime::getNow();
    dump(1);
    return 0;
}

void ObjectPoolManager::onServerTerminate()
{
    dump(2);
    for (auto it = m_mapObjectPool.begin(); it != m_mapObjectPool.end(); it ++)
    {
        it->second->unInit();
    }
    m_mapObjectPool.clear();
}

void ObjectPoolManager::recycle(const char* pObjectName, void* pData, const char* pFileName, int32_t iLine, uint64_t iHashCode)
{
    auto it = m_mapObjectPool.find(iHashCode);
    if (it != m_mapObjectPool.end())
    {
        it->second->recycle(pData, pFileName, iLine);
    }
    else
    {
        LOG_ERROR("can't find object name: " << pObjectName << ", value: " << pData);
    }
}

void ObjectPoolManager::Register(uint64_t iHashCode, ObjectPoolBase* pPool)
{
    auto it = m_mapObjectPool.find(iHashCode);
    if (it == m_mapObjectPool.end())
    {
        m_mapObjectPool[iHashCode] = pPool;
    }
}

void ObjectPoolManager::dump(uint32_t iForce)
{
    int64_t iNowTime = UtilTime::getNow();
    if (0 == iForce && iNowTime - m_iLastPrintTime < 5 * 60)
    {
        return;
    }
    m_iLastPrintTime = iNowTime;
    uint64_t iTotalSize = 0;

    for (auto it = m_mapObjectPool.begin(); it != m_mapObjectPool.end(); it ++)
    {
        it->second->dump();
        iTotalSize += it->second->getMemorySize();
    }
	std::string dumpstr;
	switch (iForce)
	{
		case 1:
			{
				dumpstr = " start";
			}
			break;
		case 2:
			{
				dumpstr = " end";
			}
			break;
		default:
			{
				dumpstr = " internal";
			}
			break;
	}
    LOG_DEBUG("total memory size: " << iTotalSize << dumpstr);
}
