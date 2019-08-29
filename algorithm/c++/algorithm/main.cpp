#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8

#include <stdio.h>
#include "sort\sort.h"
#include "maxpriority\maxpriority.h"
#include <iostream>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include "dynamic/dynamic.h"
#include "pattern/factory.h"
int main(int argc, char* argv[])
{
	std::shared_ptr<Factory> factory(new Factory);
	auto p = factory->getOne("test");
	std::cout << p->getKey() << std::endl;
	system("pause");
	return 0;
}