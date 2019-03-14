#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8

#include <stdio.h>
#include "sort\sort.h"
#include "maxpriority\maxpriority.h"
#include "skiplist\skiplist.h"

int main(int argc, char* argv[])
{
	//test sort
#if 0
	std::array<int, 5> aNums = { 8, 6, 10, 4, 1 };
	CSort<5> cc(aNums);
	//cc.InsertSort();
	//cc.SelectSort();
	//cc.MergeSort(0, aNums.size() - 1);
	//cc.Build_Max_Heap();
	//cc.Heap_Sort();
	cc.Quick_Sort(0, aNums.size() - 1);
	cc.print();
#endif // 0

	//test max priority queue
#if 0
	CMaxPriority cc;
	for (int i = 0; i < 5; ++i) {
		cc.Insert(i);
	}
	cc.Print();
	cc.Pop();
	cc.Print();
	cc.Increase(1, 10);
	cc.Print();

#endif // 0
	
	//test skiplist
#if 1
	SkipList skiplist;
	skiplist.CreateList();
	assert(skiplist.FindNode(1) == nullptr);
	skiplist.InsertNode(1);
	assert(skiplist.FindNode(1));
	skiplist.DeleteNode(1);
	assert(skiplist.FindNode(1) == nullptr);
#endif // 0

	system("pause");
	return 0;
}