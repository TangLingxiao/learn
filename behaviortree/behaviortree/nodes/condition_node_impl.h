
#ifndef _CONDITION_NODE_H_
#define _CONDITION_NODE_H_
#include "tree_node.h"
#include "base.pb.h"
namespace BT
{
	class CHPCondition :public CConditionNode
	{
	public:
		CHPCondition(){}
		virtual ~CHPCondition() = default;
		virtual ENodeType GetNodeType()const final{
			return E_TYPE_CONDITION_HP;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nNum = 0;
		ECampCondition m_eCamp = E_CAMP_OTHER;
		EOperator m_eOperator = E_OPERATOR_ERR;
		EHPCondition m_eHp = E_HP_NUM;
		bool m_bSelf = false;
	};

	class CRageCondition :public CConditionNode
	{
	public:
		CRageCondition()
		{
		}
		virtual ~CRageCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_RAGE;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nNum = 0;
		ECampCondition m_eCamp = E_CAMP_OTHER;
		EOperator m_eOperator = E_OPERATOR_ERR;
		bool m_bSelf = false;
	};

	class CAttrCondition :public CConditionNode
	{
	public:
		CAttrCondition()
		{
		}
		virtual ~CAttrCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_ATTR;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nAttrId = 0;
		EOperator m_eOperator = E_OPERATOR_ERR;
		ECounterRelation m_eRelation = COUNTER_NONE;
	};

	class CEnergyCondition :public CConditionNode
	{
	public:
		CEnergyCondition()
		{
		}
		virtual ~CEnergyCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_ENERGY;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nEnergy = 0;
		ECampCondition m_eCamp = E_CAMP_OTHER;
		EOperator m_eOperator = E_OPERATOR_ERR;
	};

	class CNumericCondition :public CConditionNode
	{
	public:
		CNumericCondition()
		{
		}
		virtual ~CNumericCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_NUMERIC;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nNum = 0;
		ECampCondition m_eCamp = E_CAMP_OTHER;
		EOperator m_eOperator = E_OPERATOR_ERR;
		ENumericCondition m_eNumeric = E_NUMERIC_ATTACK;
		bool m_bSelf = false;
	};

	class CProbCondition :public CConditionNode
	{
	public:
		CProbCondition()
		{
		}
		virtual ~CProbCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_PROB;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nNum = 0;
		EOperator m_eOperator = E_OPERATOR_LT;
	};

	class CRoundCondition :public CConditionNode
	{
	public:
		CRoundCondition()
		{
		}
		virtual ~CRoundCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_ROUND;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nNum = 0;
		EOperator m_eOperator = E_OPERATOR_ERR;
		int m_nCirculate = 0;
	};

	class CClassCondition :public CConditionNode
	{
	public:
		CClassCondition()
		{
		}
		virtual ~CClassCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_CLASS;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nNum = 0;
		EOperator m_eOperator = E_OPERATOR_ERR;
	};

	class CFighterCondition :public CConditionNode
	{
	public:
		CFighterCondition()
		{
		}
		virtual ~CFighterCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_FIGHTER;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nNum = 0;
		EOperator m_eOperator = E_OPERATOR_ERR;
		ECampCondition m_eCamp = E_CAMP_OTHER;
		EFighterCondition m_eFighter = E_FIGHTER_ID;
		bool m_bSelf = false;
	};

	class CTeamCondition :public CConditionNode
	{
	public:
		CTeamCondition()
		{
		}
		virtual ~CTeamCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_TEAM;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		ECampCondition m_eCamp = E_CAMP_OTHER;
		bool m_bHave = false;
	};

	class CLevelCondition :public CConditionNode
	{
	public:
		CLevelCondition()
		{
		}
		virtual ~CLevelCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_LEVEL;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nNum = 0;
		EOperator m_eOperator = E_OPERATOR_ERR;
		ECampCondition m_eCamp = E_CAMP_OTHER;
	};

	class CTurnCondition :public CConditionNode
	{
	public:
		CTurnCondition()
		{
		}
		virtual ~CTurnCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_TURN;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		ECampCondition m_eCamp = E_CAMP_OTHER;
	};

	class CStateCondition :public CConditionNode
	{
	public:
		CStateCondition()
		{
		}
		virtual ~CStateCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_STATE;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		ECampCondition m_eCamp = E_CAMP_OTHER;
		EEffectType m_eState = E_EFFECT_TYPE_ARMOR;
		bool m_bHave = false;
	};
}
#endif