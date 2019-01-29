#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#ifndef _BEHAVIOR_TREE_NODE_
#define _BEHAVIOR_TREE_NODE_
#include <string>
#include "basic_types.h"
namespace BT
{
	class CTreeNode
	{
	public:
		CTreeNode(const std::string& strName):m_strName(strName){}
		virtual ~CTreeNode() {}
		virtual ENodeType GetType()const = 0;
		virtual void AddChild(CTreeNode*) = 0;
		virtual ENodeStatus ExecuteTick();
		ENodeStatus GetStatus()const { return m_eStatus; }
		void SetStatus(ENodeStatus eStatus) { m_eStatus = eStatus; }
	protected:
		virtual ENodeStatus tick() = 0;
	private:
		std::string m_strName;
		ENodeStatus m_eStatus;
	};
}
#endif

