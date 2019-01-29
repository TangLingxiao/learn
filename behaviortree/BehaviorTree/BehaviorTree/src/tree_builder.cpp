#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "tree_builder.h"
#include <cassert>
using namespace BT;
void BT::CBehaviorTree::Tick()
{
	if (m_pRoot) {
		m_pRoot->ExecuteTick();
	}
}

BT::CTreeNode* BT::CBehaviorTree::SetRoot(CTreeNode* pRoot)
{
	if (pRoot && !m_pRoot) {
		m_pRoot = pRoot;
	}
	return m_pRoot;
}

CBehaviorTree * BT::CBehaviorTreeBuilder::BuildTree()
{
	while (!m_stTree.empty()) {
		m_stTree.pop();
	}
	CBehaviorTree *pTree = new CBehaviorTree();
	pTree->SetRoot(m_pTreeRoot);
	m_pTreeRoot = NULL;
	return pTree;
}

CBehaviorTreeBuilder * BT::CBehaviorTreeBuilder::Back()
{
	m_stTree.pop();
	return this;
}

bool BT::CBehaviorTreeBuilder::AddTreeNode(CTreeNode* pNode)
{
	if (pNode == NULL) {
		return false;
	}
	if (!m_pTreeRoot) {
		m_pTreeRoot = pNode;
	}
	else
	{
		if (m_stTree.empty()) {
			return false;
		}
		CTreeNode* pParent = m_stTree.top();
		if (!pParent) {
			return false;
		}
		if (pParent->GetType() == E_NODE_CONTROL || pParent->GetType() == E_NODE_LOOP) {
			pParent->AddChild(pNode);
		}
		else {
			return false;
		}
	}
	m_stTree.push(pNode);
	return true;
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateHpCondition(int nNum, EConditionCamp eCamp, EOperator eOperator, EHPCondition eHp, bool bSelf)
{
	return new CHPCondition("hp condition",nNum, eCamp, eOperator, eHp, bSelf);
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateRageCondition(int nRage, EConditionCamp eCamp, EOperator eOperator)
{
	return new CRageCondition("rage condition",nRage, eCamp, eOperator);
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateAttrCondition(int nAttrId, bool bHave)
{
	return new CAttrCondition("attr condition",nAttrId, bHave);
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateEnergyCondition(int nEnergy, EOperator eOperator)
{
	return new CEnergyCondition("energy condition", nEnergy, eOperator);
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateNumericCondition(int nNum, EConditionCamp eCamp, EOperator eOperator, ENumericCondition eNumeric, bool bSelf)
{
	return new CNumericCondition("numeric condition", nNum, eCamp, eOperator, eNumeric, bSelf);
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateProbCondition(int nNum)
{
	return new CProbCondition("prob condition", nNum);
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateExecAction()
{
	return new CExecAction("exec action");
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateSafeAction()
{
	return new CSafeAction("safe action");
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateHitAction()
{
	return new CHitAction("hit action");
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateSkillAction(int nSkillId)
{
	return new CSkillAction("skill action", nSkillId);
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateSuperAction()
{
	return new CSuperAction("super action");
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateChangeAction()
{
	return new CChangeAction("change action");
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateTeamSkillAction()
{
	return new CTeamSkillAction("teamskill action");
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateOtherAction()
{
	return new COtherAction("other action");
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateSequence()
{
	return new CSequenceNode("sequence");
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateSelect()
{
	return new CSelectNode("select");
}

CTreeNode * BT::CBehaviorTreeBuilder::CreateLoop(int nTimes)
{
	return new CLoopNode("loop");
}
