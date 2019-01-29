#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "control_node.h"

namespace BT
{
	class CSelectNode :public CControlNode
	{
	public:
		CSelectNode(const std::string& strName):CControlNode(strName){}
		virtual ~CSelectNode(){}
	private:
		virtual ENodeStatus tick();
	};
}