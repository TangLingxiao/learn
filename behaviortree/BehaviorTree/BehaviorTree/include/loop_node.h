#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "tree_node.h"

namespace BT
{
	class CLoopNode :public CTreeNode
	{
	public:
		CLoopNode(const std::string& strName):CTreeNode(strName),m_pChild(NULL), m_nTimes(3){}
		~CLoopNode() = default;
		virtual ENodeType GetType()const final {
			return E_NODE_LOOP;
		}
		void SetLoopTimes(int nTimes) {
			m_nTimes = nTimes;
		}
		virtual void AddChild(CTreeNode*)final;
	protected:
		virtual ENodeStatus tick()final;
	private:
		CTreeNode* m_pChild;
		int m_nTimes;
	};
}