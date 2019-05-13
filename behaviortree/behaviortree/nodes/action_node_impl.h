
#ifndef _ACTION_NODE_H_
#define _ACTION_NODE_H_
#include "tree_node.h"
namespace BT
{
	class CSelectHitAction :public CActionNode
	{
	public:
		CSelectHitAction() {}
		virtual ~CSelectHitAction() = default;
		ENodeType GetNodeType()const {
			return E_TYPE_ACTION_HIT;
		}
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	};

	class CSelectSkillAction :public CActionNode
	{
	public:
		CSelectSkillAction() :m_nSkillId(0) {}
		virtual ~CSelectSkillAction() = default;
		ENodeType GetNodeType()const {
			return E_TYPE_ACTION_SKILL;
		}

		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nSkillId;
	};

	class CSelectSuperAction :public CActionNode
	{
	public:
		CSelectSuperAction() {}
		virtual ~CSelectSuperAction() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_ACTION_SUPER;
		}
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	};

	class CSelectExplosionAction :public CActionNode
	{
	public:
		CSelectExplosionAction() {}
		virtual ~CSelectExplosionAction() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_ACTION_SUPER;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nSkillGroupId = 0;
	};

	class CSelectChangeAction :public CActionNode
	{
	public:
		CSelectChangeAction() {}
		virtual ~CSelectChangeAction() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_ACTION_CHANGE;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		bool CheckCanChange(CBehaviorTree* pTree, int &nAssitPos, int &nAssistHeroId);
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nHeroId = 0;
	};

	class CSelectTeamSkillAction :public CActionNode
	{
	public:
		CSelectTeamSkillAction() {}
		virtual ~CSelectTeamSkillAction() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_ACTION_TEAMSKILL;
		}
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	};

	class CSelectOtherAction :public CActionNode
	{
	public:
		CSelectOtherAction() {}
		virtual ~CSelectOtherAction() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_ACTION_OTHER;
		}
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	};

	class CExecAction :public CActionNode
	{
	public:
		CExecAction() {}
		virtual ~CExecAction() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_ACTION_EXEC;
		}
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	};

	class CSafeAction :public CActionNode
	{
	public:
		CSafeAction() {}
		virtual ~CSafeAction() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_ACTION_SAFE;
		}
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	};
}
#endif