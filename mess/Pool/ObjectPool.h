#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__

#include <deque>
#include <cassert>
#include <typeinfo>
#include "Util/TimeHelper.h"
#include "Log/Logger.h"
#include "ObjectPoolMgr.h"
#define INIT_MALLOCLIST_SIZE 100

class MallocList
{
public:
    MallocList(int32_t iMallocSize) : m_iMallocSize(iMallocSize)
    {
    }

    ~MallocList()
    {
        while (!m_freeList.empty())
        {
            free(m_freeList.front());
            m_freeList.pop_front();
        }
    }

    void init(const std::string &strObjectName, uint32_t _initSize, int32_t &iGetNum)
    {
        (void)strObjectName;
        initSize = _initSize;
        iGetNum += initSize;
        for (uint32_t i = 0; i < initSize; ++i)
        {
            void *pData = malloc(m_iMallocSize);
            assert(pData != nullptr);
            m_freeList.emplace_back(pData);
        }
#ifdef DEBUG_WQ
        LOG_DEBUG("[WQ] " << strObjectName << " 池子初始化" << initSize << " 个 ");
#endif
    }

    void *get(const std::string &strObjectName, int32_t &iGetNum)
    {
        (void)strObjectName;
        if (!m_freeList.empty())
        {
            auto *pData = m_freeList.front();
            m_freeList.pop_front();
#ifdef DEBUG_WQ
            LOG_DEBUG("[WQ] 池子里取 " << strObjectName);
#endif
            return pData;
        }

        void *pData = malloc(m_iMallocSize);
        assert(pData != nullptr);
        iGetNum += 1;
#ifdef DEBUG_WQ
        LOG_DEBUG("[WQ] 需要重新构造 " << strObjectName);
#endif
        return pData;
    }

    void recycle(void *pData, const std::string &strObjectName, int32_t iGetNum, int32_t &iRecycleNum)
    {
        (void)strObjectName;
        (void)iGetNum;
        if (m_freeList.size() < initSize)
        {
#ifdef DEBUG_WQ
            LOG_DEBUG("[WQ] 放回池子里 " << strObjectName);
#endif
            m_freeList.emplace_back(pData);
        }
        else
        {
#ifdef DEBUG_WQ
            LOG_DEBUG("[WQ] 直接释放 " << strObjectName);
#endif
            free(pData);
            iRecycleNum += 1;
        }
    }

    std::deque<void *> &getMallocList() { return m_freeList; }

private:
    int32_t m_iMallocSize{0};
    uint32_t initSize{0};
    std::deque<void *> m_freeList;
};

class ObjectPoolBase
{
public:
    ObjectPoolBase() {}
    virtual ~ObjectPoolBase() {}

    virtual void unInit() = 0;
    virtual void recycle(void *pObject, const char *strFileName, int32_t iLine) = 0;
    virtual void dump() = 0;
    virtual int32_t getMemorySize() = 0;
};

template <typename T>
class ObjectPool : ObjectPoolBase
{
    ObjectPool()
    {
        m_strObjectName = typeid(T).name();
        m_iObjectSize = sizeof(T);
        m_pMalloc = new MallocList(m_iObjectSize);
    }

public:
    virtual ~ObjectPool()
    {
        if (m_pMalloc)
        {
            delete m_pMalloc;
            m_pMalloc = nullptr;
        }
    }

    static ObjectPool<T> *getInstance()
    {
        if (!m_pSingleton)
        {
            m_pSingleton = new ObjectPool<T>();
            ObjectPoolManager::getInstance().Register((uint64_t) typeid(T).hash_code(), m_pSingleton);
        }
        return m_pSingleton;
    }

    void initObjectPool(uint32_t initSize)
    {
        m_pMalloc->init(m_strObjectName, initSize, m_iGetNum);
    }

    virtual void unInit()
    {
        if (m_pSingleton)
        {
            delete m_pSingleton;
            m_pSingleton = nullptr;
        }
    }

    template <class... Args>
    T *get(const char *strFileName, int32_t iLine, Args &&... args)
    {
        (void)strFileName;
        (void)iLine;
        void *pData = m_pMalloc->get(m_strObjectName, m_iGetNum);
        T *pObject = new (pData) T(std::forward<Args>(args)...);
        return pObject;
    }

    virtual void recycle(void *pObject, const char *strFileName, int32_t iLine)
    {
        (void)strFileName;
        (void)iLine;
        T *pData = static_cast<T *>(pObject);
        pData->~T();
        m_pMalloc->recycle(pData, m_strObjectName, m_iGetNum, m_iRecycleNum);
    }

    virtual int32_t getMemorySize() { return (m_iGetNum - m_iRecycleNum) * m_iObjectSize; }

    virtual void dump()
    {
        LOG_DEBUG("object[" << m_strObjectName << "] get[" << m_iGetNum << "], recycle[" << m_iRecycleNum << "], object size[" << m_iObjectSize << "], memory size[" << (m_iGetNum - m_iRecycleNum) * m_iObjectSize << "]");
    }

private:
    MallocList *m_pMalloc{nullptr};

    int32_t m_iGetNum{0};
    int32_t m_iRecycleNum{0};

    int32_t m_iObjectSize{0};
    std::string m_strObjectName;
    static ObjectPool<T> *m_pSingleton;
};

template <typename T>
ObjectPool<T> *ObjectPool<T>::m_pSingleton = nullptr;

#define DELETE_OBJECT(obj)                                                                                                         \
    if (nullptr != obj)                                                                                                            \
    {                                                                                                                              \
        ObjectPoolManager::getInstance().recycle(typeid(*obj).name(), obj, __FILE__, (int32_t)__LINE__, typeid(*obj).hash_code()); \
        obj = nullptr;                                                                                                             \
    }

#define NEW_OBJECT(ClassName, ...) ObjectPool<ClassName>::getInstance()->get(__FILE__, (int32_t)__LINE__, ##__VA_ARGS__)
#define INIT_OBJECT_POOL(ClassName, initSize) ObjectPool<ClassName>::getInstance()->initObjectPool(initSize)

#endif
