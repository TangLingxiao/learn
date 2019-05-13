#include "condition_node_impl.h"
#include "basic_funcs.h"
#include "utility.h"

using namespace BT;

bool BT::CRageCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pCamp = pNode->Attribute("camp");
	const char* pOper = pNode->Attribute("operator");
	const char* pNum = pNode->Attribute("num");
	const char* pSelf = pNode->Attribute("self");
	if (!pCamp || !pOper || !pNum || !pSelf) {
		return false;
	}
	m_bSelf = false;
	if (atoi(pSelf) != 0) {
		m_bSelf = true;
	}
	m_eCamp = (ECampCondition)atoi(pCamp);
	m_eOperator = GetOperator(pOper);
	if (m_eOperator == E_OPERATOR_ERR) {
		return false;
	}
	m_nNum = atoi(pNum);
	return true;
}

BT::ENodeStatus BT::CRageCondition::tick(CBehaviorTree* pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	CBattleRole* pTargetRole = GetTargetRole(pCurCreature, m_eCamp);
	if (!pTargetRole) {
		return E_NODE_FAILURE;
	}
	for (int i = 0; i < MAX_FORMATION_CUR_HERO; ++i)
	{
		CCreature* pCreature = pTargetRole->FindCreatureByPos(i);
		if (!pCreature || pCreature->IsDead()) {
			continue;
		}
		if (m_bSelf) {
			m_nNum = 0;
		}
		bool bRet = CheckResult(m_eOperator, 0, m_nNum);
		if (bRet) {
			SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]rage条件判断成功，行为树id: %d, 节点名字: %s, creatureid: %d, creature: %p", pTree->GetTreeID(),this->GetNodeName(), pCreature->GetTemplateId(), pCreature);
			return E_NODE_SUCCESS;
		}
	}
	
	return E_NODE_FAILURE;
}

bool BT::CHPCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pCamp = pNode->Attribute("camp");
	const char* pOper = pNode->Attribute("operator");
	const char* pNum = pNode->Attribute("num");
	const char* pParam = pNode->Attribute("param");
	const char* pSelf = pNode->Attribute("self");
	if (!pCamp || !pOper || !pNum || !pParam || !pSelf) {
		return false;
	}
	m_eCamp = (ECampCondition)atoi(pCamp);
	m_eOperator = GetOperator(pOper);
	if (m_eOperator == E_OPERATOR_ERR) {
		return false;
	}
	m_bSelf = false;
	if (atoi(pSelf) != 0) {
		m_bSelf = true;
	}
	m_nNum = atoi(pNum);
	m_eHp = E_HP_NUM;
	if (pParam) {
		if (StrEqual(pParam, "percent")) {
			m_eHp = E_HP_PERCENT;
		}
		else if (StrEqual(pParam, "num")) {
			m_eHp = E_HP_NUM;
		}
		else {
			return false;
		}
	}
	return true;
}

BT::ENodeStatus BT::CHPCondition::tick(CBehaviorTree* pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	CBattleRole* pTargetRole = GetTargetRole(pCurCreature, m_eCamp);
	if (!pTargetRole) {
		return E_NODE_FAILURE;
	}
	for (int i = 0; i < MAX_FORMATION_CUR_HERO; ++i)
	{
		CCreature* pCreature = pTargetRole->FindCreatureByPos(i);
		if (!pCreature || pCreature->IsDead()) {
			continue;
		}
		if (m_eHp == E_HP_NUM)
		{
			if (m_bSelf) {
				m_nNum = pCurCreature->GetHP();
			}
			bool bRet = CheckResult(m_eOperator, pCreature->GetHP(), m_nNum);
			if (bRet) {
				SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]hp num 条件判断成功, 行为树id: %d,节点名字: %s, creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(), pCreature->GetTemplateId(), pCreature);
				return E_NODE_SUCCESS;
			}
		}
		else
		{
			if (m_bSelf) {
				int nMaxHP = pCurCreature->GetBattle_HP(0, 0);
				if (nMaxHP == 0) {
					return E_NODE_FAILURE;
				}
				m_nNum = pCurCreature->GetHP() * 100 / nMaxHP;
			}
			int nTargetMaxHp = pCreature->GetBattle_HP(0, 0);
			if (nTargetMaxHp == 0) {
				return E_NODE_FAILURE;
			}
			int nTargetPercent = pCreature->GetHP() * 100 / nTargetMaxHp;
			bool bRet = CheckResult(m_eOperator, nTargetPercent, m_nNum);
			if (bRet) {
				SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]hp percent 条件判断成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(), pCreature->GetTemplateId(), pCreature);
				return E_NODE_SUCCESS;
			}
		}
	}

	return E_NODE_FAILURE;
}

bool BT::CAttrCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pOper = pNode->Attribute("operator");
	const char* pId = pNode->Attribute("attrid");
	const char* pParam = pNode->Attribute("param");
	if (!pOper || !pId || !pParam) {
		return false;
	}
	m_eOperator = GetOperator(pOper);
	if (m_eOperator == E_OPERATOR_ERR) {
		return false;
	}
	m_nAttrId = atoi(pId);
	if (StrEqual(pParam, "positive")) {
		m_eRelation = COUNTER_POSSIVE;
	}
	else if (StrEqual(pParam, "negative")) {
		m_eRelation = COUNTER_NEGATIVE;
	}
	return true;
}

BT::ENodeStatus BT::CAttrCondition::tick(CBehaviorTree* pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	CBattleRole* pTargetRole = GetTargetRole(pCurCreature, E_CAMP_OTHER);
	if (!pTargetRole) {
		return E_NODE_FAILURE;
	}
	IBattle *pBattle = pCurCreature->GetBattle();
	if (!pBattle) {
		return E_NODE_FAILURE;
	}

	for (int i = 0; i < MAX_FORMATION_CUR_HERO; ++i)
	{
		CCreature* pCreature = pTargetRole->FindCreatureByPos(i);
		if (!pCreature || pCreature->IsDead()) {
			continue;
		}
		if (m_eRelation == COUNTER_NONE)
		{
			bool bRet = CheckResult(m_eOperator, pCreature->GetCounterType(), m_nAttrId);
			if (bRet) {
				SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]attr 条件判断成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(), pCreature->GetTemplateId(), pCreature);
				return E_NODE_SUCCESS;
			}
		}
		else {
			ECounterRelation eRet = pBattle->CheckCounterRelation(pCurCreature, pCreature);
			if (eRet == m_eRelation) {
				SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]attr 条件判断成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(), pCreature->GetTemplateId(), pCreature);
				return E_NODE_SUCCESS;
			}
		}
	}

	return E_NODE_FAILURE;
}

bool BT::CEnergyCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pOperator = pNode->Attribute("operator");
	const char* pNum = pNode->Attribute("num");
	if (!pOperator || !pNum) {
		return false;
	}
	m_nEnergy = atoi(pNum);
	m_eOperator = GetOperator(pOperator);
	if (m_eOperator == E_OPERATOR_ERR) {
		return false;
	}
	const char* pCamp = pNode->Attribute("camp");
	if (pCamp) {
		m_eCamp = (ECampCondition)atoi(pCamp);
	}
	return true;
}

BT::ENodeStatus BT::CEnergyCondition::tick(CBehaviorTree* pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	CBattleRole* pTargetRole = GetTargetRole(pCurCreature, m_eCamp);
	if (!pTargetRole) {
		return E_NODE_FAILURE;
	}
	int nEnergy = pTargetRole->m_nFuryBean;
	bool bRet = CheckResult(m_eOperator, nEnergy, m_nEnergy);
	if (bRet) {
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]energy 条件判断成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(),pCurCreature->GetTemplateId(), pCurCreature);
		return E_NODE_SUCCESS;
	}
	return E_NODE_FAILURE;
}

bool BT::CNumericCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pCamp = pNode->Attribute("camp");
	const char* pOper = pNode->Attribute("operator");
	const char* pNum = pNode->Attribute("num");
	const char* pParam = pNode->Attribute("param");
	const char* pSelf = pNode->Attribute("self");
	if (!pCamp || !pOper || !pNum || !pParam || !pSelf) {
		return false;
	}
	m_eCamp = (ECampCondition)atoi(pCamp);
	m_bSelf = false;
	if (atoi(pSelf) != 0) {
		m_bSelf = true;
	}
	m_nNum = atoi(pNum);
	m_eOperator = GetOperator(pOper);
	if (m_eOperator == E_OPERATOR_ERR) {
		return false;
	}
	m_eNumeric = E_NUMERIC_ATTACK;
	if (StrEqual(pParam, "attack")) {
		m_eNumeric = E_NUMERIC_ATTACK;
	}
	else if (StrEqual(pParam, "def")) {
		m_eNumeric = E_NUMERIC_DEF;
	}
	else if (StrEqual(pParam, "cri")) {
		m_eNumeric = E_NUMERIC_CRI;
	}
	else if (StrEqual(pParam, "domage")) {
		m_eNumeric = E_NUMERIC_DOMAGE;
	}
	else if (StrEqual(pParam, "speed")) {
		m_eNumeric = E_NUMERIC_SPEED;
	}
	else {
		return false;
	}

	return true;
}

BT::ENodeStatus BT::CNumericCondition::tick(CBehaviorTree* pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	CBattleRole* pTargetRole = GetTargetRole(pCurCreature, m_eCamp);
	if (!pTargetRole) {
		return E_NODE_FAILURE;
	}
	for (int i = 0; i < MAX_FORMATION_CUR_HERO; ++i)
	{
		CCreature* pCreature = pTargetRole->FindCreatureByPos(i);
		if (!pCreature || pCreature->IsDead()) {
			continue;
		}
		if (m_bSelf) {
			int nNum = GetNumeric(pCurCreature, m_eNumeric);
			if (nNum != -1) {
				m_nNum = nNum;
			}
		}
		int nTargetNum = GetNumeric(pCreature, m_eNumeric);
		if (nTargetNum < 0) {
			return E_NODE_FAILURE;
		}
		bool bRet = CheckResult(m_eOperator, nTargetNum, m_nNum);
		if (bRet) {
			SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]numeric 条件判断成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(),pCreature->GetTemplateId(), pCreature);
			return E_NODE_SUCCESS;
		}
	}

	return E_NODE_FAILURE;
}

bool BT::CProbCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pNum = pNode->Attribute("num");
	if (!pNum) {
		return false;
	}
	m_nNum = atoi(pNum);

	return true;
}

BT::ENodeStatus BT::CProbCondition::tick(CBehaviorTree* pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	int nProb = GNDP::GetRand(1, 100);
	bool bRet = CheckResult(m_eOperator, nProb, m_nNum);
	if (bRet) {
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]prob 条件判断成功,行为树id: %d,节点名字: %s, creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(),pCurCreature->GetTemplateId(), pCurCreature);
		return E_NODE_SUCCESS;
	}
	return E_NODE_FAILURE;
}

bool BT::CRoundCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pNum = pNode->Attribute("num");
	const char* pOper = pNode->Attribute("operator");
	if (!pNum || !pOper) {
		return false;
	}
	m_nNum = atoi(pNum);
	m_eOperator = GetOperator(pOper);
	if (m_eOperator == E_OPERATOR_ERR) {
		return false;
	}
	const char* pCir = pNode->Attribute("circulate");
	if (pCir) {
		m_nCirculate = atoi(pCir);
	}
	return true;
}

ENodeStatus BT::CRoundCondition::tick(CBehaviorTree * pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	IBattle* pBattle = pCurCreature->GetBattle();
	if (!pBattle) {
		return E_NODE_FAILURE;
	}
	int nRound = pBattle->GetCurRound() / 2;
	if (m_nCirculate > 0) {
		nRound %= m_nCirculate;
	}
	bool bRet = CheckResult(m_eOperator, nRound, m_nNum);
	
	if (bRet) {
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]round 条件判断成功,行为树id: %d,节点名字: %s, creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(),pCurCreature->GetTemplateId(), pCurCreature);
		return E_NODE_SUCCESS;
	}
	return E_NODE_FAILURE;
}

bool BT::CClassCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pNum = pNode->Attribute("num");
	const char* pOper = pNode->Attribute("operator");
	if (!pNum || !pOper) {
		return false;
	}
	m_nNum = atoi(pNum);

	m_eOperator = GetOperator(pOper);
	if (m_eOperator == E_OPERATOR_ERR) {
		return false;
	}
	
	return true;
}

ENodeStatus BT::CClassCondition::tick(CBehaviorTree * pTree)
{
	return ENodeStatus();
}

bool BT::CFighterCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pNum = pNode->Attribute("num");
	const char* pOper = pNode->Attribute("operator");
	const char* pCamp = pNode->Attribute("camp");
	const char* pParam = pNode->Attribute("param");
	if (!pCamp || !pOper || !pNum || !pParam) {
		return false;
	}
	m_nNum = atoi(pNum);
	m_eCamp = (ECampCondition)atoi(pCamp);
	if (StrEqual(pParam, "count")) {
		m_eFighter = E_FIGHTER_COUNT;
	}
	else if (StrEqual(pParam, "id")) {
		m_eFighter = E_FIGHTER_ID;
	}
	else {
		return false;
	}

	m_eOperator = GetOperator(pOper);
	if (m_eOperator == E_OPERATOR_ERR) {
		return false;
	}
	
	const char* pSelf = pNode->Attribute("self");
	if (pSelf && atoi(pSelf) != 0) {
		m_bSelf = true;
	}

	return true;
}

ENodeStatus BT::CFighterCondition::tick(CBehaviorTree * pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	CBattleRole* pTargetRole = GetTargetRole(pCurCreature, m_eCamp);
	if (!pTargetRole) {
		return E_NODE_FAILURE;
	}
	int nCount = 0;
	if (m_eFighter == E_FIGHTER_ID && m_bSelf) {
		bool bRet = CheckResult(m_eOperator, pCurCreature->GetTemplateId(), m_nNum);
		if (bRet) {
			SNK_LOGGER_ASYNC_DEBUG("[行为树相关]fighter id 条件判断成功,行为树id: %d,节点名字: %s, creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature);
			return E_NODE_SUCCESS;
		}
		else
		{
			return E_NODE_FAILURE;
		}
	}
	for (int i = 0; i < MAX_FORMATION_CUR_HERO; ++i)
	{
		CCreature* pCreature = pTargetRole->FindCreatureByPos(i);
		if (!pCreature || pCreature->IsDead()) {
			continue;
		}
		++nCount;
		if (m_eFighter == E_FIGHTER_ID) {
			bool bRet = CheckResult(m_eOperator, pCreature->GetTemplateId(), m_nNum);
			if (bRet) {
				SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]fighter id 条件判断成功,行为树id: %d,节点名字: %s, creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(),pCreature->GetTemplateId(), pCreature);
				return E_NODE_SUCCESS;
			}
		}
	}
	if (m_eFighter == E_FIGHTER_COUNT) {
		bool bRet = CheckResult(m_eOperator, nCount, m_nNum);
		if (bRet) {
			SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]fighter count 条件判断成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(),pCurCreature->GetTemplateId(), pCurCreature);
			return E_NODE_SUCCESS;
		}
	}
	return E_NODE_FAILURE;
}

bool BT::CTeamCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pCamp = pNode->Attribute("camp");
	const char* pHave = pNode->Attribute("have");
	if (!pCamp || !pHave) {
		return false;
	}
	m_eCamp = (ECampCondition)atoi(pCamp);
	m_bHave = (bool)atoi(pHave);
	return true;
}

ENodeStatus BT::CTeamCondition::tick(CBehaviorTree * pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	CBattleRole* pTargetRole = GetTargetRole(pCurCreature, m_eCamp);
	if (!pTargetRole) {
		return E_NODE_FAILURE;
	}
	for (int i = 0; i < MAX_FORMATION_CUR_HERO; ++i)
	{
		CCreature* pCreature = pTargetRole->FindCreatureByPos(i);
		if (!pCreature || pCreature->IsDead()) {
			continue;
		}
		const std::map<uint32, CSkill*>& mapSkills = pCreature->GetSkill();
		for (const auto& it : mapSkills) {
			CSkill* pSkill = it.second;
			if (!pSkill) {
				return E_NODE_FAILURE;
			}
			if (pSkill->GetGroup() == E_SKILL_GROUP_ASSIST) {
				int nRet = -1; //pSkill->CheckCoopSkillUse(*pCreature);
				if (nRet == 0) {
					SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]team 条件判断成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(),pCreature->GetTemplateId(), pCreature);
					return E_NODE_SUCCESS;
				}
			}
		}
	}

	return E_NODE_FAILURE;	
}

bool BT::CLevelCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pCamp = pNode->Attribute("camp");
	const char* pOper = pNode->Attribute("operator");
	const char* pNum = pNode->Attribute("num");
	if (!pCamp || !pOper || !pNum) {
		return false;
	}
	m_eCamp = (ECampCondition)atoi(pCamp);
	m_nNum = atoi(pNum);
	m_eOperator = GetOperator(pOper);
	if (m_eOperator == E_OPERATOR_ERR) {
		return false;
	}

	return true;
}

ENodeStatus BT::CLevelCondition::tick(CBehaviorTree * pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	CBattleRole* pTargetRole = GetTargetRole(pCurCreature, m_eCamp);
	if (!pTargetRole) {
		return E_NODE_FAILURE;
	}
	for (int i = 0; i < MAX_FORMATION_CUR_HERO; ++i)
	{
		CCreature* pCreature = pTargetRole->FindCreatureByPos(i);
		if (!pCreature || pCreature->IsDead()) {
			continue;
		}
		int nLevel = pCreature->GetLevel();
		bool bRet = CheckResult(m_eOperator, nLevel, m_nNum);
		if (bRet) {
			SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]level 条件判断成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(),pCreature->GetTemplateId(), pCreature);
			return E_NODE_SUCCESS;
		}
	}
	return E_NODE_FAILURE;
}

bool BT::CTurnCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pCamp = pNode->Attribute("camp");
	if (!pCamp) {
		return false;
	}
	m_eCamp = (ECampCondition)atoi(pCamp);
	return true;
}

ENodeStatus BT::CTurnCondition::tick(CBehaviorTree * pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	IBattle* pBattle = pCurCreature->GetBattle();
	if (!pBattle) {
		return E_NODE_FAILURE;
	}
	int nFirstRoundCamp = pBattle->GetFirstRoundCamp();//先手阵营
	int nCurCreatureCamp = pCurCreature->GetCamp();//我方阵营
	if ((nCurCreatureCamp == nFirstRoundCamp && m_eCamp == E_CAMP_SELF) || 
		(nCurCreatureCamp != nFirstRoundCamp && m_eCamp == E_CAMP_OTHER)) {
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]turn 条件判断成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(),pCurCreature->GetTemplateId(), pCurCreature);
		return E_NODE_SUCCESS;
	}
	return E_NODE_FAILURE;
}

bool BT::CStateCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pCamp = pNode->Attribute("camp");
	const char* pParam = pNode->Attribute("param");
	const char* pHave = pNode->Attribute("have");
	if (!pCamp || !pHave || !pParam) {
		return false;
	}
	m_eCamp = (ECampCondition)atoi(pCamp);
	m_bHave = (bool)atoi(pHave);
	m_eState = (EEffectType)atoi(pParam);
	/*if (StrEqual(pParam, "immune")) {
		m_eState = E_EFFECT_TYPE_IMMUNE;
	}
	else if (StrEqual(pParam, "dizzy")) {
		m_eState = E_EFFECT_TYPE_DIZZY;
	}
	else if (StrEqual(pParam, "freeze")) {
		m_eState = E_EFFECT_TYPE_FREEZE;
	}
	else if (StrEqual(pParam, "numb")) {
		m_eState = E_EFFECT_TYPE_BENUMB;
	}
	else if (StrEqual(pParam, "armor")) {
		m_eState = E_EFFECT_TYPE_ARMOR;
	}
	else if (StrEqual(pParam, "silence")) {
		m_eState = E_EFFECT_TYPE_SILENCE;
	}
	else if (StrEqual(pParam, "burying")) {
		m_eState = E_EFFECT_TYPE_FLIMSY_BURYING;
	}
	else {
		return false;
	}*/
	return true;
}

ENodeStatus BT::CStateCondition::tick(CBehaviorTree * pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	CBattleRole* pTargetRole = GetTargetRole(pCurCreature, m_eCamp);
	if (!pTargetRole) {
		return E_NODE_FAILURE;
	}
	for (int i = 0; i < MAX_FORMATION_CUR_HERO; ++i)
	{
		CCreature* pCreature = pTargetRole->FindCreatureByPos(i);
		if (!pCreature || pCreature->IsDead()) {
			continue;
		}
		
		if (pCreature->HasBuffType(m_eState) == m_bHave) {
			SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]state 条件判断成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(),pCreature->GetTemplateId(), pCreature);
			return E_NODE_SUCCESS;
		}
	}
	return E_NODE_FAILURE;
}
