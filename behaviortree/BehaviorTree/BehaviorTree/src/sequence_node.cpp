#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "sequence_node.h"
using namespace BT;

ENodeStatus BT::CSequenceNode::tick()
{
	const auto &vecChildren = GetChildren();
	for (size_t i = 0; i < vecChildren.size(); ++i) {
		CTreeNode* pChild = vecChildren[i];
		if(NULL == pChild)
			continue;
		const ENodeStatus eStatus = pChild->ExecuteTick();
		switch (eStatus)
		{
		case BT::E_NODE_SUCCESS:
			continue;
			break;
		case BT::E_NODE_FAILURE:
		{
			return eStatus;
			break;
		}
		default:
			break;
		}
	}
	return E_NODE_SUCCESS;
}
