#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "control_node.h"

namespace BT
{
	class CSequenceNode :public CControlNode
	{
	public:
		CSequenceNode(const std::string& strName):CControlNode(strName){}
		virtual ~CSequenceNode(){}
	private:
		virtual ENodeStatus tick();
	};
}