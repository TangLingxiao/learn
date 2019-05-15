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

int main(int argc, char* argv[])
{
	IRankList oList;
	oList.Init();
	
	int times = 10005;
	for (int i = 0; i < times; ++i)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(1, 10000);
		auto nScore = dis(gen);
		oList.InsertItem(i + 1, nScore);
	}
	

	std::vector<SRankItem> vecItems;
	auto print = [&](int32 begin, int32 end) {
		std::cout << " =============begin: " << begin << " end:" << end << "===============================" << std::endl;
		vecItems.clear();
		oList.GetRankItems(begin, end, vecItems);
		for (auto & one : vecItems)
		{
			std::cout << "id: " << one.nId << " score: " << one.nScore << " rank: " << one.nRank << std::endl;
		}
		std::cout << " ==============================================================" << std::endl;
	};

	uint64 nRank = oList.GetRank(1);
	std::cout << "nRank:" << nRank << std::endl;
	
	system("pause");
	return 0;
}