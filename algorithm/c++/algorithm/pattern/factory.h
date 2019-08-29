#ifndef __FACTORY_H__
#define __FACTORY_H__
#include <memory>
#include <mutex>
#include <string>
#include <map>
#include "singleton.h"

struct MyStruct
{
	MyStruct(std::string key):key(key){}
	std::string key;
	const std::string& getKey() { return key; }
};
class Factory :public std::enable_shared_from_this<Factory>
{
public:
	std::shared_ptr<MyStruct> getOne(const std::string& key)
	{
		std::shared_ptr<MyStruct> t;
		std::lock_guard<std::mutex> lock(mutex_);
		std::weak_ptr<MyStruct>& weak = mapS[key];
		t = weak.lock();
		if (!t)
		{
			t.reset(new MyStruct(key), std::bind(&Factory::deleteCallBack, std::weak_ptr<Factory>(shared_from_this()), std::placeholders::_1));
			weak = t;
		}
		return t;
	}


private:
	static void deleteCallBack(const std::weak_ptr<Factory>& weakptr, MyStruct* t)
	{
		auto shareptr(weakptr.lock());
		if (shareptr)
		{
			shareptr->remove(t);
		}
		delete t;
	}

	void remove(MyStruct* t)
	{
		if (t)
		{
			std::lock_guard<std::mutex> lock(mutex_);
			mapS.erase(t->getKey());
		}
	}
	
	std::mutex mutex_;
	std::map<std::string, std::weak_ptr<MyStruct>> mapS;
};
#endif
