#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8

#include <stdio.h>
#include "sort\sort.h"
#include "maxpriority\maxpriority.h"
#include "skiplist\rankmgr.h"
#include <iostream>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include "dynamic/dynamic.h"
int main(int argc, char* argv[])
{
	CMaxIncreaseLength o;
	std::vector<int> v{ 10,9,2,8,3,7,4,6,5 };
	std::array<std::array<int, ArraySize>, ArraySize> temp{ 0 };
	o.Init(std::move(v));
	std::vector<int> res;
	std::cout << o.Cal(temp, res) << std::endl;

	for (int i = 0; i < ArraySize; ++i)
	{
		for (int j = 0; j < ArraySize; ++j)
		{
			std::cout << temp[i][j] << " ";
		}
		std::cout << std::endl;
	}

	for (size_t i = 0; i < res.size(); ++i)
	{
		std::cout << res[i] << " ";
	}
	std::cout << std::endl;
	system("pause");
	return 0;
}