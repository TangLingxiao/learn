#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "control_node.h"

using namespace BT;

void BT::CControlNode::AddChild(CTreeNode * pChild)
{
	if (!pChild) {
		return;
	}

	m_vecChildren.push_back(pChild);
}

const std::vector<CTreeNode*>& BT::CControlNode::GetChildren() const
{
	return m_vecChildren;
}

const CTreeNode * BT::CControlNode::GetChild(int index)
{
	return m_vecChildren.at(index);
}

const int BT::CControlNode::GetChildrenNum() const
{
	return m_vecChildren.size();
}
