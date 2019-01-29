#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "tree_node.h"
#include <vector>
namespace BT
{
	class CControlNode :public CTreeNode
	{
	public:
		CControlNode(const std::string& strName):CTreeNode(strName){
			m_vecChildren.clear();
		}
		virtual ~CControlNode(){}

		virtual ENodeType GetType()const final{
			return E_NODE_CONTROL;
		}
		virtual void AddChild(CTreeNode* pChild) final;

		const std::vector<CTreeNode*>&GetChildren()const;

		const CTreeNode* GetChild(int index);

		const int GetChildrenNum()const;
	private:
		std::vector<CTreeNode*> m_vecChildren;
	};
}