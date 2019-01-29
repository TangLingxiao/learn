#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "tree_node.h"
#include <functional>

namespace BT
{
	class CActionNode :public CTreeNode
	{
	public:
		CActionNode(const std::string& strName):CTreeNode(strName) {}
		virtual ~CActionNode() {}
		virtual ENodeType GetType()const final {
			return E_NODE_ACTION;
		}
		virtual void AddChild(CTreeNode*)final{}
	};

	class CHitAction :public CActionNode
	{
	public:
		CHitAction(const std::string& strName):CActionNode(strName) {}
		virtual ~CHitAction() = default;
	protected:
		virtual ENodeStatus tick();
	};

	class CSkillAction :public CActionNode
	{
	public:
		CSkillAction(const std::string& strName, int nskillid):CActionNode(strName), m_nSkillId(nskillid) {}
		virtual ~CSkillAction() = default;
	protected:
		virtual ENodeStatus tick();
	private:
		int m_nSkillId;
	};

	class CSuperAction :public CActionNode
	{
	public:
		CSuperAction(const std::string& strName):CActionNode(strName) {}
		virtual ~CSuperAction() = default;
	protected:
		virtual ENodeStatus tick();
	};

	class CChangeAction :public CActionNode
	{
	public:
		CChangeAction(const std::string& strName):CActionNode(strName) {}
		virtual ~CChangeAction() = default;
	protected:
		virtual ENodeStatus tick();
	};

	class CTeamSkillAction :public CActionNode
	{
	public:
		CTeamSkillAction(const std::string& strName):CActionNode(strName) {}
		virtual ~CTeamSkillAction() = default;
	protected:
		virtual ENodeStatus tick();
	};

	class COtherAction :public CActionNode
	{
	public:
		COtherAction(const std::string& strName):CActionNode(strName) {}
		virtual ~COtherAction() = default;
	protected:
		virtual ENodeStatus tick();
	};

	class CExecAction :public CActionNode
	{
	public:
		CExecAction(const std::string& strName) :CActionNode(strName) {}
		virtual ~CExecAction() = default;
	protected:
		virtual ENodeStatus tick();
	};

	class CSafeAction :public CActionNode
	{
	public:
		CSafeAction(const std::string& strName) :CActionNode(strName) {}
		virtual ~CSafeAction() = default;
	protected:
		virtual ENodeStatus tick();
	};
}