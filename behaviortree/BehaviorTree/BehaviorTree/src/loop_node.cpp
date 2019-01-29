#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "loop_node.h"
using namespace BT;

void BT::CLoopNode::AddChild(CTreeNode *pChild)
{
	m_pChild = pChild;
}

ENodeStatus BT::CLoopNode::tick()
{
	for (int i = 0; i < m_nTimes; ++i) {
		ENodeStatus eStatus = m_pChild->ExecuteTick();
		if (eStatus == E_NODE_SUCCESS) {
			return eStatus;
		}
	}
	return E_NODE_FAILURE;
}
