#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "tree_node.h"

using namespace BT;
BT::ENodeStatus BT::CTreeNode::ExecuteTick()
{
	const ENodeStatus eStatus = tick();
	SetStatus(eStatus);
	return eStatus;
}


