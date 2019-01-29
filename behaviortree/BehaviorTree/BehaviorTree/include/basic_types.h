#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#ifndef _BEHAVIOR_TREE_TYPES
#define _BEHAVIOR_TREE_TYPES

namespace BT
{
	enum ENodeType
	{
		E_NODE_NONE = 0,
		E_NODE_ACTION,
		E_NODE_CONDITION,
		E_NODE_CONTROL,
		E_NODE_LOOP
	};

	enum ENodeStatus
	{
		E_NODE_SUCCESS,
		E_NODE_FAILURE,
	};

	enum EHPCondition
	{
		E_HP_NUM,
		E_HP_PERCENT,
	};

	enum EConditionCamp
	{
		E_CAMP_SELF = 0,
		E_CAMP_OTHER,
	};

	enum ENumericCondition
	{
		E_NUMERIC_ATTACK,
		E_NUMERIC_DEF,
		E_NUMERIC_CRI,
		E_NUMERIC_DOMAGE,
		E_NUMERIC_SPEED,
	};

	enum EOperator
	{
		E_OPERATOR_ERR,
		E_OPERATOR_LT,//<
		E_OPERATOR_LE,//<=
		E_OPERATOR_EQ,//==
		E_OPERATOR_GT,//>
		E_OPERATOR_GE,//>=
		E_OPERATOR_NE,//!=
	};
}

#endif