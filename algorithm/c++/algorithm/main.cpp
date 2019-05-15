#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8

#include <stdio.h>
#include "sort\sort.h"
#include "maxpriority\maxpriority.h"
#include "skiplist\skiplist.h"
#include <iostream>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#define SKIPLIST_MAXLEVEL (32)
int main(int argc, char* argv[])
{
	CSkipList o;
	o.CreateList();
	auto begin1 = std::clock();
	int times = 10000;
	for (int i = 0; i < times; ++i)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(1, 10000);
		auto nScore = dis(gen);
		o.Insert(i + 1, nScore);
	}
	auto end1 = std::clock();
	auto dur1 = end1 - begin1;
	struct SNode
	{
		SNode(uint64 n1 = 0, int32 n2 = 0)
		{
			nkey = n1;
			nScore = n2;
		}
		uint64 nkey = 0;
		int32 nScore = 0;
	};
	std::vector<SNode>vecNodes;
	auto begin2 = std::clock();
	for (int i = 0; i < times; ++i)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(1, 10000);
		auto nScore = dis(gen);
		vecNodes.emplace_back(SNode(i + 1, nScore));
		//std::sort(vecNodes.begin(), vecNodes.end(), [](SNode& o1, SNode& o2) {
		//	return o1.nScore < o2.nScore;
		//});
	}
	
	auto end2 = std::clock();
	auto dur2 = end2 - begin2;
	std::cout << "duration1: " << dur1 << std::endl << "duration2: " << dur2 << std::endl;
	std::cout << "length1:" << o.GetLength()<< std::endl << "length2:" << vecNodes.size() << std::endl;
	std::cout << "size1:" << o.GetSize() << std::endl << "size2:" << vecNodes.size()*sizeof(SNode) << std::endl;
	system("pause");
	return 0;
}