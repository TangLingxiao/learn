#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "include/tree_builder.h"
#include <stdio.h>
#include "xmlparser/xmlparser.h"
using namespace BT;
const char* path = "E:\\learn\\behaviortree\\BehaviorTree\\ai.xml";
int main()
{
	CBehaviorTreeBuilder *bt = new CBehaviorTreeBuilder();
	CXMLParser oParser;
	oParser.Init(bt);
	bool bRet = oParser.LoadAndBuildTree(path);
	CBehaviorTree *pTree = bt->BuildTree();
	pTree->Tick();
	system("pause");
	exit(0);
}