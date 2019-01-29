#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "action_node.h"
#include "condition_node.h"
#include "decorator_node.h"
#include "select_node.h"
#include "sequence_node.h"
#include "loop_node.h"
#include <stack>
namespace BT
{
	class CBehaviorTree
	{
	public:
		CBehaviorTree():m_pRoot(NULL){}
		~CBehaviorTree(){}
		void Tick();
		bool HaveRoot() { return m_pRoot ? true : false; }
		CTreeNode* SetRoot(CTreeNode* pRoot);
	private:
		CTreeNode* m_pRoot;
	};

	class CBehaviorTreeBuilder
	{
	public:
		CBehaviorTreeBuilder() :m_pTreeRoot(NULL),m_stTree() {}
		~CBehaviorTreeBuilder(){}
		CTreeNode* CreateHpCondition(int nNum, EConditionCamp eCamp, EOperator eOperator, EHPCondition eHp, bool bSelf);
		CTreeNode* CreateRageCondition(int nRage, EConditionCamp eCamp, EOperator eOperator);
		CTreeNode* CreateAttrCondition(int nAttrId, bool bHave);
		CTreeNode* CreateEnergyCondition(int nEnergy, EOperator eOperator);
		CTreeNode* CreateNumericCondition(int nNum, EConditionCamp eCamp, EOperator eOperator, ENumericCondition eNumeric, bool bSelf);
		CTreeNode* CreateProbCondition(int nNum);
		CTreeNode* CreateExecAction();
		CTreeNode* CreateSafeAction();
		CTreeNode* CreateHitAction();
		CTreeNode* CreateSkillAction(int nSkillId);
		CTreeNode* CreateSuperAction();
		CTreeNode* CreateChangeAction();
		CTreeNode* CreateTeamSkillAction();
		CTreeNode* CreateOtherAction();
		CTreeNode* CreateSequence();
		CTreeNode* CreateSelect();
		CTreeNode* CreateLoop(int nTimes);
		CBehaviorTree* BuildTree();
		CBehaviorTreeBuilder* Back();
		bool AddTreeNode(CTreeNode* pNode);
	private:
		CTreeNode* m_pTreeRoot;
		std::stack<CTreeNode*> m_stTree;
	};

}