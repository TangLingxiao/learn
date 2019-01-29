#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "decorator_node.h"

using namespace BT;

void BT::CDecoratorNode::AddChild(CTreeNode * pChild)
{
	m_pChild = pChild;
}

const CTreeNode * BT::CDecoratorNode::GetChilde() const
{
	return m_pChild;
}

