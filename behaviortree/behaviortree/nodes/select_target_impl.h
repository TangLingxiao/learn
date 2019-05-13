#ifndef _SELECT_TARGET_H_
#define _SELECT_TARGET_H_
#include "tree_node.h"
#include "base.pb.h"
namespace BT
{
	class CSelectHPCondition :public CConditionNode
	{
	public:
		CSelectHPCondition() {}
		virtual ~CSelectHPCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_SELECT_HP;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		ECampCondition m_eCamp = E_CAMP_OTHER;
		EHPCondition m_eHp = E_HP_NUM;
		ERangeCondition m_eRange = E_RANGE_MAX;
		EOperator m_eOper = E_OPERATOR_ERR;
		int m_nNum = 0;
	};

	class CSelectRageCondition :public CConditionNode
	{
	public:
		CSelectRageCondition()
		{
		}
		virtual ~CSelectRageCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_SELECT_RAGE;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		ECampCondition m_eCamp = E_CAMP_OTHER;
		ERangeCondition m_eRange = E_RANGE_MAX;
	};

	class CSelectAttrCondition :public CConditionNode
	{
	public:
		CSelectAttrCondition()
		{
		}
		virtual ~CSelectAttrCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_SELECT_ATTR;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nAttrId = 0;
		EOperator m_eOperator = E_OPERATOR_ERR;
		ECounterRelation m_eRelation = COUNTER_NONE;
	};

	class CSelectNumericCondition :public CConditionNode
	{
	public:
		CSelectNumericCondition()
		{
		}
		virtual ~CSelectNumericCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_SELECT_NUMERIC;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		ECampCondition m_eCamp = E_CAMP_OTHER;
		ENumericCondition m_eNumeric = E_NUMERIC_ATTACK;
		ERangeCondition m_eRange = E_RANGE_MAX;
	};

	class CSelectProbCondition :public CConditionNode
	{
	public:
		CSelectProbCondition()
		{
		}
		virtual ~CSelectProbCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_SELECT_PROB;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nNum = 0;
		EOperator m_eOperator = E_OPERATOR_LT;
	};

	class CSelectFighterCondition :public CConditionNode
	{
	public:
		CSelectFighterCondition()
		{
		}
		virtual ~CSelectFighterCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_SELECT_FIGHTER;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		ECampCondition m_eCamp = E_CAMP_OTHER;
		int m_nHeroId = 0;
		bool m_bSelect = false;
	};


	class CSelectLevelCondition :public CConditionNode
	{
	public:
		CSelectLevelCondition()
		{
		}
		virtual ~CSelectLevelCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_SELECT_LEVEL;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		ECampCondition m_eCamp = E_CAMP_OTHER;
		ERangeCondition m_eRange = E_RANGE_MAX;
	};

	class CSelectStateCondition :public CConditionNode
	{
	public:
		CSelectStateCondition()
		{
		}
		virtual ~CSelectStateCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_SELECT_STATE;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		ECampCondition m_eCamp = E_CAMP_OTHER;
		EEffectType m_eState = E_EFFECT_TYPE_ARMOR;
		bool m_bHave = false;
	};

	class CSelectLocationCondition :public CConditionNode
	{
	public:
		CSelectLocationCondition()
		{
		}
		virtual ~CSelectLocationCondition() = default;
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_CONDITION_SELECT_RAGE;
		}
		virtual bool LoadXML(XMLElement* pNode);
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	public:
		int m_nPos = E_POS_FRONT;
	};
}
#endif
