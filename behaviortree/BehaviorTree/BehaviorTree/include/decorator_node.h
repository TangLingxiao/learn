#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "tree_node.h"
#include <functional>
namespace BT
{
	class CDecoratorNode :public CTreeNode
	{
	public:
		CDecoratorNode(const std::string& strName):CTreeNode(strName){}
		virtual ~CDecoratorNode(){}
		virtual ENodeType GetType()const final {
			//return E_NODE_DECORATOR;
		}

		virtual void AddChild(CTreeNode* pChild);
		const CTreeNode* GetChilde()const;
	private:
		CTreeNode* m_pChild;
	};
}