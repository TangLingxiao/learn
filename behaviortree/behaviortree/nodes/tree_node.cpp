#include <algorithm>
#include "tree_node.h"
#include "utility.h"
using namespace BT;

BT::ENodeStatus BT::ITreeNode::ExecuteTick(CBehaviorTree* pTree)
{
	if (!pTree) {
		SNK_LOGGER_ASYNC_CRITICAL( "[行为树相关] 行为树不存在");
		return E_NODE_FAILURE;
	}
	const ENodeStatus eStatus = tick(pTree);
	SetNodeStatus(eStatus);
	
	if (eStatus == E_NODE_FAILURE) {
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关] 节点失败，行为树id: %d, 节点名字: %s", pTree->GetTreeID(), this->GetNodeName());
	}
	return eStatus;
}
void BT::CompositeNode::AddChild(ITreeNode * pChild)
{
	if (!pChild) {
		return;
	}

	m_vecChildren.push_back(pChild);
}

const std::vector<ITreeNode*>& BT::CompositeNode::GetChildren() const
{
	return m_vecChildren;
}

const ITreeNode * BT::CompositeNode::GetChild(int index)
{
	return m_vecChildren.at(index);
}

const int BT::CompositeNode::GetChildrenNum() const
{
	return m_vecChildren.size();
}

//void BT::CLoopNode::AddChild(ITreeNode *pChild)
//{
//	m_pChild = pChild;
//}
//
//BT::ENodeStatus BT::CLoopNode::tick(CBehaviorTree* pTree)
//{
//	for (int i = 0; i < m_eCamp; ++i) {
//		ENodeStatus eStatus = m_pChild->ExecuteTick(pTree);
//		if (eStatus == E_NODE_SUCCESS) {
//			return eStatus;
//		}
//	}
//	return E_NODE_FAILURE;
//}

BT::ENodeStatus BT::CBehaviorTree::Tick()
{
	if (m_pRoot) {
		return m_pRoot->ExecuteTick(this);
	}
	return E_NODE_FAILURE;
}

BT::ITreeNode* BT::CBehaviorTree::SetTreeRoot(ITreeNode* pRoot)
{
	if (pRoot && !m_pRoot) {
		m_pRoot = pRoot;
	}
	return m_pRoot;
}

int BT::CBehaviorTree::GetTargetObjId()
{
	if (m_setTargetsObjId.empty()) {
		return -1;
	}
	int nRand = GNDP::GetRand(0, m_setTargetsObjId.size() - 1);
	auto it = m_setTargetsObjId.begin();
	for (int i = 0; i < nRand; ++i) {
		++it;
	}
	return *it;
}

void BT::CBehaviorTree::ClearTargetObjId()
{
	m_setTargetsObjId.clear();
}

bool BT::CBehaviorTree::SetTargetObjId(std::set<int> &setTargets)
{
	//返回false的出口要把m_setTargetsObjId清掉
	if (setTargets.empty()) {
		m_setTargetsObjId.clear();
		return false;
	}
	//如果是空则拷贝
	if (m_setTargetsObjId.empty()) {
		m_setTargetsObjId = setTargets;
		return true;
	}
	//非空则取交集，如果交集为空则返回false
	auto setTemp = m_setTargetsObjId;
	m_setTargetsObjId.clear();
	std::set_intersection(setTemp.begin(), setTemp.end(), setTargets.begin(), setTargets.end(), std::inserter(m_setTargetsObjId, m_setTargetsObjId.begin()));
	if (m_setTargetsObjId.empty()) {
		return false;
	}
	return true;

}

#if VERSION_CE
void BT::CBehaviorTree::AddSelectAction(ESelectAction eAction)
{
	if (eAction == E_SELECT_NONE) {
		return;
	}
	if (m_aSelectAction[0] == E_SELECT_NONE) {
		m_aSelectAction[0] = eAction;
	}
	else if(m_aSelectAction[1] == E_SELECT_NONE){
		//如果有两个，则必然有一个change，如果没有，则重置为eAction
		if ((m_aSelectAction[0] == E_SELECT_CHANGE && eAction != E_SELECT_CHANGE)
			|| (m_aSelectAction[0] != E_SELECT_CHANGE && eAction == E_SELECT_CHANGE)) {
			m_aSelectAction[1] = eAction;
		}
		else {
			m_aSelectAction[0] = eAction;
		}
	}
	else {
		//进到这里说明上次动作选择失败，重置为eAction
		m_aSelectAction[0] = eAction;
		m_aSelectAction[1] = E_SELECT_NONE;
	}

}
#endif

void BT::CBehaviorTree::ClearTree()
{
	m_setTargetsObjId.clear();
	m_nSkillType = 0;
	m_eSelectAction = E_SELECT_NONE;
	
	m_pOwner = nullptr;
	m_pSkillCfg = nullptr;
	
	m_nAssitPos = 0;//援护的目标
#if VERSION_CE
	m_aSelectAction = { E_SELECT_NONE };
#endif
}

