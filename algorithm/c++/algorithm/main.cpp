#include <thread>
#include "log/logmgr.h"
#include "observer.h"
#include "factory.h"
#include "test/blockingqueue_test.h"

void test1()
{
	std::shared_ptr<Factory<MyStruct>> factory(new Factory<MyStruct>);
	{
		auto p = factory->getOne("test1");
	}
}

void test2()
{
	std::shared_ptr<MyStruct> st;
	{
		std::shared_ptr<Factory<MyStruct>> factory(new Factory<MyStruct>);
		st = factory->getOne("test2");
	}
}

int main(int argc, char* argv[])
{
	if (!LogMgr::getInstance().init("algo.log", "./", true))
	{
		exit(-1);
	}
	////////////////////////////////////////////////////

	testBlockingQueue();


	////////////////////////////////////////////////////
	LogMgr::getInstance().release();
	system("pause");
	return 0;
}
