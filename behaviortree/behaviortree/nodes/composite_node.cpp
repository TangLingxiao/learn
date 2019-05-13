
#include "composite_node.h"
using namespace BT;

BT::ENodeStatus BT::CSequenceNode::tick(CBehaviorTree* pTree)
{
	const auto &vecChildren = GetChildren();
	for (size_t i = 0; i < vecChildren.size(); ++i) {
		ITreeNode* pChild = vecChildren[i];
		if (NULL == pChild)
			continue;
		const ENodeStatus eStatus = pChild->ExecuteTick(pTree);
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

BT::ENodeStatus BT::CSelectNode::tick(CBehaviorTree* pTree)
{
	const auto &vecChildren = GetChildren();
	for (size_t i = 0; i < vecChildren.size(); ++i) {
		ITreeNode* pChild = vecChildren[i];
		if (NULL == pChild)
			continue;
		const ENodeStatus eStatus = pChild->ExecuteTick(pTree);
		switch (eStatus)
		{
		case BT::E_NODE_SUCCESS:
			return eStatus;
			break;
		case BT::E_NODE_FAILURE:
		{
			continue;
			break;
		}
		default:
			break;
		}
	}
	return E_NODE_FAILURE;
}
