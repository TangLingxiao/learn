
#ifndef _COMPOSITE_NODE_H_
#define _COMPOSITE_NODE_H_
#include "tree_node.h"
namespace BT
{
	//序列，依次执行所有子节点，只要一个节点返回失败就结束//用于实现逻辑与关系
	class CSequenceNode :public CompositeNode
	{
	public:
		CSequenceNode(){}
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_SEQUENCE;
		}
		virtual ~CSequenceNode() {}
	private:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	};

	//选择，依次执行所有子节点，只要一个节点返回成功就结束//用于实现逻辑或关系
	class CSelectNode :public CompositeNode
	{
	public:
		CSelectNode(){}
		virtual ~CSelectNode() {}
		virtual ENodeType GetNodeType()const final {
			return E_TYPE_SELECT;
		}
	private:
		virtual ENodeStatus tick(CBehaviorTree* pTree);
	};

}
#endif