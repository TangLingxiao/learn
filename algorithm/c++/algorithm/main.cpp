#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8

#include <stdio.h>
#include "sort\sort.h"
#include "maxpriority\maxpriority.h"
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include "dynamic/dynamic.h"
#include "pattern/factory.h"
#include <thread>
#include "log/logmgr.h"

int main(int argc, char* argv[])
{
	gLogMgr.reset(new LogMgr);
	if (!gLogMgr->init("algo.log", "./"))
	{
		exit(-1);
	}

	auto f = [](std::shared_ptr<Factory<MyStruct>>& factory, const std::string& key) {
		auto one = factory->getOne(key);
		LOG_DEBUG("getkey"<<one->getKey());
	};
	std::shared_ptr<Factory<MyStruct>> factory(new Factory<MyStruct>);
	std::thread t1(f, factory, "test1");
	std::thread t2(f, factory, "test2");
	t1.join();
	t2.join();

	gLogMgr->release();
	system("pause");
	return 0;
}