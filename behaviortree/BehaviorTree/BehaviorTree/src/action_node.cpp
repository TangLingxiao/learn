#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#include "action_node.h"
using namespace BT;

ENodeStatus BT::CHitAction::tick()
{
	printf("hit action\n");
	return E_NODE_SUCCESS;
}

ENodeStatus BT::CSkillAction::tick()
{
	printf("skill action\n");
	return E_NODE_SUCCESS;
}

ENodeStatus BT::CSuperAction::tick()
{
	printf("super action\n");
	return E_NODE_SUCCESS;
}

ENodeStatus BT::CChangeAction::tick()
{
	printf("change action\n");
	return E_NODE_SUCCESS;
}

ENodeStatus BT::CTeamSkillAction::tick()
{
	printf("team skill action\n");
	return E_NODE_SUCCESS;
}

ENodeStatus BT::COtherAction::tick()
{
	printf("other action\n");
	return E_NODE_SUCCESS;
}

ENodeStatus BT::CExecAction::tick()
{
	printf("exec action\n");
	return E_NODE_SUCCESS;
}

ENodeStatus BT::CSafeAction::tick()
{
	printf("safe action\n");
	return E_NODE_SUCCESS;
}
