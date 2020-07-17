#pragma once

#include <unordered_map>
#include <list>

template <typename FuncType>
struct CallInfo
{
    FuncType _func;
    CallInfo(FuncType func)
    {
        _func = func;
    }
    CallInfo(const CallInfo &right)
    {
        _func = right._func;
    }

    CallInfo &operator=(const CallInfo &right)
    {
        _func = right._func;
        return *this;
    }
    bool operator==(const CallInfo &right) { return _func == right._func; };
};
template <typename T, typename FuncType, typename... Args>
class TCallBack
{
    typedef CallInfo<FuncType> CallUnit;
    typedef std::list<CallUnit> CallLists;
    typedef std::unordered_map<T, CallLists> CallMap;

public:
    TCallBack(){};
    ~TCallBack(){};

    bool RegisterCallBack(const T &id, const FuncType callFunc)
    {
        CallUnit unit(callFunc);
        auto iter = _callMap.find(id);
        if (iter == _callMap.end())
        {
            auto ret = _callMap.insert(std::make_pair(id, CallLists()));
            iter = ret.first;
        }
        iter->second.push_back(unit);
        return true;
    }

    bool UnRegisterCallBack(const T &id, const FuncType callFunc)
    {
        CallUnit unit(callFunc);
        auto iter = _callMap.find(id);
        if (iter != _callMap.end())
        {
            auto findIter = std::find(iter->second.begin(), iter->second.end(), unit);
            if (findIter != iter->second.end())
            {
                iter->second.erase(findIter);
                return true;
            }
        }
        return false;
    }

    bool ClearCallBack(const T &id)
    {
        auto iter = _callMap.find(id);
        if (iter != _callMap.end())
            iter->second.clear();

        return true;
    }

    bool CallWithCondition(const T &id, bool condition, Args&&... args)
    {
        auto iter = _callMap.find(id);
        if (iter == _callMap.end())
            return true;
        for (auto unit : iter->second)
        {
            if (condition == unit._func(std::forward<Args>(args)...))
                return condition;
        }

        return !condition;
    }

    void Call(const T &id, Args&&... args)
    {
        auto iter = _callMap.find(id);
        if (iter != _callMap.end())
        {
            for (auto unit : iter->second)
                unit._func(std::forward<Args>(args)...);
        }
    }

protected:
private:
    CallMap _callMap;
};