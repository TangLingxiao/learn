#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "tree_node.h"
#include <functional>
namespace BT
{
	class CConditionNode :public CTreeNode
	{
	public:
		CConditionNode(const std::string& strName) :CTreeNode(strName) {}
		virtual ~CConditionNode() {}
		virtual ENodeType GetType()const final {
			return E_NODE_CONDITION;
		}
		virtual void AddChild(CTreeNode*)final {}
	};

	class CHPCondition :public CConditionNode
	{
	public:
		CHPCondition(const std::string& strName, int nNum, EConditionCamp eCamp, EOperator eOperator, EHPCondition eHp, bool bSelf) :CConditionNode(strName)
		{
			m_nNum = nNum;
			m_eCamp = eCamp;
			m_eOperator = eOperator;
			m_eHp = eHp;
			m_bSelf = bSelf;
		}
		virtual ~CHPCondition() = default;
	protected:
		virtual ENodeStatus tick();
	private:
		int m_nNum;
		EConditionCamp m_eCamp;
		EOperator m_eOperator;
		EHPCondition m_eHp;
		bool m_bSelf;
	};

	class CRageCondition :public CConditionNode
	{
	public:
		CRageCondition(const std::string& strName, int nNum, EConditionCamp eCamp, EOperator eOperator) :CConditionNode(strName)
		{
			m_nNum = nNum;
			m_eCamp = eCamp;
			m_eOperator = eOperator;
		}
		virtual ~CRageCondition() = default;
	protected:
		virtual ENodeStatus tick();
	private:
		int m_nNum;
		EConditionCamp m_eCamp;
		EOperator m_eOperator;
	};

	class CAttrCondition :public CConditionNode
	{
	public:
		CAttrCondition(const std::string& strName, int nAttrId, bool bHave) :CConditionNode(strName)
		{
			m_nAttrId = nAttrId;
			m_bHave = bHave;
		}
		virtual ~CAttrCondition() = default;
	protected:
		virtual ENodeStatus tick();
	private:
		int m_nAttrId;
		bool m_bHave;
	};

	class CEnergyCondition :public CConditionNode
	{
	public:
		CEnergyCondition(const std::string& strName, int nEnergy, EOperator eOperator) :CConditionNode(strName)
		{
			m_nEnergy = nEnergy;
			m_eOperator = eOperator;
		}
		virtual ~CEnergyCondition() = default;
	protected:
		virtual ENodeStatus tick();
	private:
		int m_nEnergy;
		EOperator m_eOperator;
	};

	class CNumericCondition :public CConditionNode
	{
	public:
		CNumericCondition(const std::string& strName, int nNum, EConditionCamp eCamp, EOperator eOperator, ENumericCondition eNumeric, bool bSelf) :CConditionNode(strName)
		{
			m_nNum = nNum;
			m_eCamp = eCamp;
			m_eOperator = eOperator;
			m_eNumeric = eNumeric;
			m_bSelf = bSelf;
		}
		virtual ~CNumericCondition() = default;
	protected:
		virtual ENodeStatus tick();
	private:
		int m_nNum;
		EConditionCamp m_eCamp;
		EOperator m_eOperator;
		ENumericCondition m_eNumeric;
		bool m_bSelf;
	};

	class CProbCondition :public CConditionNode
	{
	public:
		CProbCondition(const std::string& strName, int nNum) :CConditionNode(strName)
		{
			m_nNum = nNum;
		}
		virtual ~CProbCondition() = default;
	protected:
		virtual ENodeStatus tick();
	private:
		int m_nNum;
	};
}