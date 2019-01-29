#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "condition_node.h"

using namespace BT;

ENodeStatus BT::CRageCondition::tick()
{
	printf("rage condition\n");
	return E_NODE_SUCCESS;
}

ENodeStatus BT::CHPCondition::tick()
{
	printf("hp condition\n");
	return E_NODE_SUCCESS;
}

ENodeStatus BT::CAttrCondition::tick()
{
	printf("attr condition\n");
	return E_NODE_SUCCESS;
}

ENodeStatus BT::CEnergyCondition::tick()
{
	printf("energy condition\n");
	return E_NODE_SUCCESS;
}

ENodeStatus BT::CNumericCondition::tick()
{
	printf("numeric condition\n");
	return E_NODE_SUCCESS;
}

ENodeStatus BT::CProbCondition::tick()
{
	printf("prob condition\n");
	return E_NODE_SUCCESS;
}
