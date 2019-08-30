#ifndef __FACTORY_H__
#define __FACTORY_H__
#include <memory>
#include <mutex>
#include <string>
#include <map>
#include "logmgr.h"
struct MyStruct
{
	MyStruct(std::string key):key(key){}
	~MyStruct()
	{
	}
	std::string key;
	const std::string& getKey() { return key; }
};

template<typename T>
class Factory :public std::enable_shared_from_this<Factory<T>>
{
public:
	~Factory()
	{
		LOG_DEBUG("release factory");
	}
	std::shared_ptr<T> getOne(const std::string& key)
	{
		std::shared_ptr<T> t;
		
		{
			std::lock_guard<std::mutex> lock(mutex_);
			std::weak_ptr<T>& weak = mapS[key];
			t = weak.lock();
			if (!t)
			{
				t.reset(new T(key), std::bind(&Factory::deleteCallBack, std::weak_ptr<Factory>(shared_from_this()), std::placeholders::_1));
				weak = t;
			}
		}
		LOG_DEBUG("get key:" << key);
		return t;
	}


private:
	static void deleteCallBack(const std::weak_ptr<Factory>& weakptr, T* t)
	{
		auto shareptr(weakptr.lock());
		if (shareptr)
		{
			shareptr->remove(t);
		}
		LOG_DEBUG("delete key:" << t->getKey());
		delete t;
	}

	void remove(T* t)
	{
		if (t)
		{
			std::lock_guard<std::mutex> lock(mutex_);
			mapS.erase(t->getKey());
		}
	}
	
	std::mutex mutex_;
	std::map<std::string, std::weak_ptr<T>> mapS;
};
#endif
