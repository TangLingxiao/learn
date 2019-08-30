#include <thread>
#include "log/logmgr.h"
#include "observer.h"
#include "factory.h"
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
	if (!LogMgr::getInstance().init("algo.log", "./"))
	{
		exit(-1);
	}
	////////////////////////////////////////////////////


	test2();

	////////////////////////////////////////////////////
	LogMgr::getInstance().release();
	system("pause");
	return 0;
}
