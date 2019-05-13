#include "select_target_impl.h"
#include "basic_funcs.h"
#include "utility.h"

using namespace BT;
bool BT::CSelectHPCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pCamp = pNode->Attribute("camp");
	const char* pParam = pNode->Attribute("param");
	const char* pRange = pNode->Attribute("range");
	const char* pOper = pNode->Attribute("operator");
	const char* pNum = pNode->Attribute("num");
	if (!pCamp || !pParam || !pRange || !pOper || !pNum) {
		return false;
	}
	m_eCamp = (ECampCondition)atoi(pCamp);

	if (StrEqual(pRange, "max")) {
		m_eRange = E_RANGE_MAX;
	}
	else if (StrEqual(pRange, "min")) {
		m_eRange = E_RANGE_MIN;
	}
	else if (StrEqual(pRange, "num")) {
		m_eRange = E_RANGE_NUM;
	}
	else {
		return false;
	}
	if (StrEqual(pParam, "percent")) {
		m_eHp = E_HP_PERCENT;
	}
	else if (StrEqual(pParam, "num")) {
		m_eHp = E_HP_NUM;
	}
	else {
		return false;
	}
	m_nNum = atoi(pNum);
	m_eOper = GetOperator(pOper);
	if (m_eOper == E_OPERATOR_ERR) {
		return false;
	}
	return true;
}
ENodeStatus BT::CSelectHPCondition::tick(CBehaviorTree * pTree)
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
	IBattle *pBattle = pCurCreature->GetBattle();
	if (!pBattle) {
		return E_NODE_FAILURE;
	}
	std::vector<std::pair<int, int> >vecTargets;
	std::set<int> setTargets;

	std::vector<CCreature*> vecTemp;
	int nRet = pBattle->OptionalTargetGet(*pCurCreature, vecTemp, pTree->GetSkillCfg());
	if (nRet != 0) {
		return E_NODE_FAILURE;
	}
	for (const auto& pCreature:vecTemp)
	{
		if (!pCreature || pCreature->IsDead()) {
			continue;
		}
		
		int nNum = 0;
		if (m_eHp == E_HP_NUM) {
			nNum = pCreature->GetHP();
		}
		else {
			int nMaxHp = pCreature->GetBattle_HP(0, 0);
			if (nMaxHp == 0) {
				return E_NODE_FAILURE;
			}
			nNum = pCreature->GetHP() * 100 / nMaxHp;
		}
		if (m_eRange == E_RANGE_NUM) {
			bool bRet = CheckResult(m_eOper, nNum, m_nNum);
			if (bRet) {
				setTargets.insert(pCreature->GetObjID());
			}
			
		}
		else {
			vecTargets.push_back(std::make_pair(pCreature->GetObjID(), nNum));
		}
	}
	if (m_eRange != E_RANGE_NUM) {
		int nTarget = GetTarget(vecTargets, m_eRange);
		if (-1 == nTarget) {
			return E_NODE_FAILURE;
		}
		setTargets.insert(nTarget);
	}
	if (pTree->SetTargetObjId(setTargets)) {
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]hp目标选择成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature);
		return E_NODE_SUCCESS;
	}
	return E_NODE_FAILURE;
}

bool BT::CSelectRageCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pCamp = pNode->Attribute("camp");
	const char* pRange = pNode->Attribute("range");
	if (!pCamp || !pRange) {
		return false;
	}
	m_eCamp = (ECampCondition)atoi(pCamp);
	if (StrEqual(pRange, "max")) {
		m_eRange = E_RANGE_MAX;
	}
	else if (StrEqual(pRange, "min")) {
		m_eRange = E_RANGE_MIN;
	}
	else {
		return false;
	}
	return true;
}

ENodeStatus BT::CSelectRageCondition::tick(CBehaviorTree * pTree)
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
	IBattle *pBattle = pCurCreature->GetBattle();
	if (!pBattle) {
		return E_NODE_FAILURE;
	}
	std::vector<std::pair<int, int> >vecTargets;
	std::set<int> setTargets;

	std::vector<CCreature*> vecTemp;
	int nRet = pBattle->OptionalTargetGet(*pCurCreature, vecTemp, pTree->GetSkillCfg());
	if (nRet != 0) {
		return E_NODE_FAILURE;
	}
	for (const auto& pCreature : vecTemp)
	{
		if (!pCreature || pCreature->IsDead()) {
			continue;
		}
		vecTargets.push_back(std::make_pair(pCreature->GetObjID(), 0));
	}

	int nTarget = GetTarget(vecTargets, m_eRange);

	if (-1 == nTarget) {
		return E_NODE_FAILURE;
	}
	
	std::set<int> setTemp = { nTarget };
	if (pTree->SetTargetObjId(setTemp)) {
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]rage目标选择成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p, targetid: %d", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature, nTarget);
		return E_NODE_SUCCESS;
	}
	return E_NODE_FAILURE;
}

bool BT::CSelectAttrCondition::LoadXML(XMLElement * pNode)
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

ENodeStatus BT::CSelectAttrCondition::tick(CBehaviorTree * pTree)
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
	std::set<int> setTargets;

	std::vector<CCreature*> vecTemp;
	int nRet = pBattle->OptionalTargetGet(*pCurCreature, vecTemp, pTree->GetSkillCfg());
	if (nRet != 0) {
		return E_NODE_FAILURE;
	}
	for (const auto& pCreature : vecTemp)
	{
		if (!pCreature || pCreature->IsDead() ) {
			continue;
		}
		
		if (m_eRelation == COUNTER_NONE) {
			bool bRet = CheckResult(m_eOperator, pCreature->GetCounterType(), m_nAttrId);
			if (bRet) {
				setTargets.insert(pCreature->GetObjID());
			}
		}
		else {
			ECounterRelation eRet1 = pBattle->CheckCounterRelation(pCurCreature, pCreature);
			ECounterRelation eRet2 = pBattle->CheckCounterRelation(pCreature, pCurCreature);
			if ((eRet1 != COUNTER_NONE && eRet2 == eRet1) || eRet1 == m_eRelation) {
				setTargets.insert(pCreature->GetObjID());
			}
		}
	}
	if (pTree->SetTargetObjId(setTargets)) {
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]attr目标选择成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature);
		return E_NODE_SUCCESS;
	}
	return E_NODE_FAILURE;
}

bool BT::CSelectNumericCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pCamp = pNode->Attribute("camp");
	const char* pParam = pNode->Attribute("param");
	const char* pRange = pNode->Attribute("range");
	if (!pCamp || !pParam || !pRange) {
		return false;
	}
	m_eCamp = (ECampCondition)atoi(pCamp);
	
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
	
	if (StrEqual(pRange, "max")) {
		m_eRange = E_RANGE_MAX;
	}
	else if (StrEqual(pRange, "min")) {
		m_eRange = E_RANGE_MIN;
	}
	else {
		return false;
	}
	return true;
}

ENodeStatus BT::CSelectNumericCondition::tick(CBehaviorTree * pTree)
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
	IBattle *pBattle = pCurCreature->GetBattle();
	if (!pBattle) {
		return E_NODE_FAILURE;
	}
	std::vector<std::pair<int, int> >vecTargets;
	std::set<int> setTargets;

	std::vector<CCreature*> vecTemp;
	int nRet = pBattle->OptionalTargetGet(*pCurCreature, vecTemp, pTree->GetSkillCfg());
	if (nRet != 0) {
		return E_NODE_FAILURE;
	}
	for (const auto& pCreature : vecTemp)
	{
		if (!pCreature || pCreature->IsDead() ) {
			continue;
		}
		
		int nNum = GetNumeric(pCreature, m_eNumeric);
		if (nNum == -1) {
			return E_NODE_FAILURE;
		}
		vecTargets.push_back(std::make_pair(pCreature->GetObjID(), nNum));
	}
	
	int	nTarget = GetTarget(vecTargets, m_eRange);
	
	
	if (-1 == nTarget) {
		return E_NODE_FAILURE;
	}

	std::set<int> setTemp = { nTarget };
	if (pTree->SetTargetObjId(setTemp)) {
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]numeric目标选择成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p, targetid: %d", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature, nTarget);
		return E_NODE_SUCCESS;
	}
	return E_NODE_FAILURE;
}

bool BT::CSelectProbCondition::LoadXML(XMLElement * pNode)
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

ENodeStatus BT::CSelectProbCondition::tick(CBehaviorTree * pTree)
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
	int nRand = GNDP::GetRand(1, 100);
	bool bRet = CheckResult(m_eOperator, nRand, m_nNum);
	if (!bRet) {
		return E_NODE_FAILURE;
	}
	IBattle *pBattle = pCurCreature->GetBattle();
	if (!pBattle) {
		return E_NODE_FAILURE;
	}
	std::set<int> setTargets;

	std::vector<CCreature*> vecTemp;
	int nRet = pBattle->OptionalTargetGet(*pCurCreature, vecTemp, pTree->GetSkillCfg());
	if (nRet != 0) {
		return E_NODE_FAILURE;
	}
	for (const auto& pCreature : vecTemp)
	{
		if (!pCreature || pCreature->IsDead() ) {
			continue;
		}
		setTargets.insert(pCreature->GetObjID());
	}
	
	
	if (pTree->SetTargetObjId(setTargets)) {
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]prob目标选择成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature);
		return E_NODE_SUCCESS;
	}
	return E_NODE_FAILURE;
	
}

bool BT::CSelectFighterCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pCamp = pNode->Attribute("camp");
	const char* pHeroId = pNode->Attribute("heroid");
	const char* pSelect = pNode->Attribute("select");
	if (!pCamp || !pHeroId || !pSelect) {
		return false;
	}
	m_eCamp = (ECampCondition)atoi(pCamp);
	m_nHeroId = atoi(pHeroId);
	m_bSelect = (bool)atoi(pSelect);
	return true;
}

ENodeStatus BT::CSelectFighterCondition::tick(CBehaviorTree * pTree)
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
	IBattle *pBattle = pCurCreature->GetBattle();
	if (!pBattle) {
		return E_NODE_FAILURE;
	}
	std::set<int> setTargets;

	std::vector<CCreature*> vecTemp;
	int nRet = pBattle->OptionalTargetGet(*pCurCreature, vecTemp, pTree->GetSkillCfg());
	if (nRet != 0) {
		return E_NODE_FAILURE;
	}
	for (const auto& pCreature : vecTemp)
	{
		if (!pCreature || pCreature->IsDead()) {
			continue;
		}
		if (pCreature->GetTemplateId() == (uint32)m_nHeroId && m_bSelect) {
			setTargets.insert(pCreature->GetObjID());
			break;
		}
		else if (pCreature->GetTemplateId() != (uint32)m_nHeroId && !m_bSelect) {
			setTargets.insert(pCreature->GetObjID());
		}
	}
	if (pTree->SetTargetObjId(setTargets)) {
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]fighter目标选择成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature);
		return E_NODE_SUCCESS;
	}
	return E_NODE_FAILURE;
}

bool BT::CSelectLevelCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pCamp = pNode->Attribute("camp");
	const char* pRange = pNode->Attribute("range");
	if (!pCamp || !pRange) {
		return false;
	}
	m_eCamp = (ECampCondition)atoi(pCamp);
	if (StrEqual(pRange, "max")) {
		m_eRange = E_RANGE_MAX;
	}
	else if (StrEqual(pRange, "min")) {
		m_eRange = E_RANGE_MIN;
	}
	else {
		return false;
	}

	return true;
}

ENodeStatus BT::CSelectLevelCondition::tick(CBehaviorTree * pTree)
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
	IBattle *pBattle = pCurCreature->GetBattle();
	if (!pBattle) {
		return E_NODE_FAILURE;
	}
	std::vector<std::pair<int, int> >vecTargets;
	std::set<int> setTargets;

	std::vector<CCreature*> vecTemp;
	int nRet = pBattle->OptionalTargetGet(*pCurCreature, vecTemp, pTree->GetSkillCfg());
	if (nRet != 0) {
		return E_NODE_FAILURE;
	}
	for (const auto& pCreature : vecTemp)
	{
		if (!pCreature || pCreature->IsDead() ) {
			continue;
		}
		
		int nLevel = pCreature->GetLevel();
		vecTargets.push_back(std::make_pair(pCreature->GetObjID(), nLevel));
	}
	
	int	nTarget = GetTarget(vecTargets, m_eRange);
	
	if (-1 == nTarget) {
		return E_NODE_FAILURE;
	}

	std::set<int> setTemp = { nTarget };
	if (pTree->SetTargetObjId(setTemp)) {
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]level目标选择成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p, targetid: %d", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature, nTarget);
		return E_NODE_SUCCESS;
	}
	return E_NODE_FAILURE;
}

bool BT::CSelectStateCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pCamp = pNode->Attribute("camp");
	const char* pParam = pNode->Attribute("param");
	const char* pHave = pNode->Attribute("have");
	if (!pCamp || !pParam || !pHave) {
		return false;
	}
	m_bHave = (bool)atoi(pHave);
	m_eCamp = (ECampCondition)atoi(pCamp);
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

ENodeStatus BT::CSelectStateCondition::tick(CBehaviorTree * pTree)
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
	std::set<int> setTargets;

	std::vector<CCreature*> vecTemp;
	int nRet = pBattle->OptionalTargetGet(*pCurCreature, vecTemp, pTree->GetSkillCfg());
	if (nRet != 0) {
		return E_NODE_FAILURE;
	}
	for (const auto& pCreature : vecTemp)
	{
		if (!pCreature || pCreature->IsDead()) {
			continue;
		}
		if (pCreature->HasBuffType(m_eState) == m_bHave) {
			setTargets.insert(pCreature->GetObjID());
		}
	}
	if (pTree->SetTargetObjId(setTargets)) {
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]state目标选择成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature);
		return E_NODE_SUCCESS;
	}
	return E_NODE_FAILURE;
}

bool BT::CSelectLocationCondition::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pPos = pNode->Attribute("pos");
	if (!pPos) {
		return false;
	}
	m_nPos = (EPosCondition)(atoi(pPos) - 1);
	
	return true;
}

ENodeStatus BT::CSelectLocationCondition::tick(CBehaviorTree * pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	
	IBattle *pBattle = pCurCreature->GetBattle();
	if (!pBattle) {
		return E_NODE_FAILURE;
	}
	CBattleRole* pRoleCamp0 = pBattle->GetPlayerByCamp(CAMP_TYPE_0);
	CBattleRole* pRoleCamp1 = pBattle->GetPlayerByCamp(CAMP_TYPE_1);
	if (!(pRoleCamp1 && pRoleCamp0)) {
		return E_NODE_FAILURE;
	}
	std::set<CCreature*> setTemp;
	const int nPos = m_nPos % MAX_FORMATION_CUR_HERO;
	if (m_nPos >= MAX_FORMATION_CUR_HERO) {
		CCreature* pCreature0 = pRoleCamp0->FindAssistCreatureByPos(nPos);
		if (pCreature0) {
			setTemp.insert(pCreature0);
		}
		CCreature* pCreature1 = pRoleCamp1->FindAssistCreatureByPos(nPos);
		if (pCreature1) {
			setTemp.insert(pCreature1);
		}
	}
	else {
		CCreature* pCreature0 = pRoleCamp0->FindCreatureByPos(nPos);
		if (pCreature0) {
			setTemp.insert(pCreature0);
		}
		CCreature* pCreature1 = pRoleCamp1->FindCreatureByPos(nPos);
		if (pCreature1) {
			setTemp.insert(pCreature1);
		}
	}
	if (setTemp.empty()) {
		return E_NODE_FAILURE;
	}
	std::set<int> setTargets;

	std::vector<CCreature*> vecTemp;
	int nRet = pBattle->OptionalTargetGet(*pCurCreature, vecTemp, pTree->GetSkillCfg());
	if (nRet != 0) {
		return E_NODE_FAILURE;
	}
	for (const auto& pCreature : vecTemp)
	{
		if (!pCreature || pCreature->IsDead()) {
			continue;
		}
		if (setTemp.find(pCreature)!=setTemp.end()) {
			setTargets.insert(pCreature->GetObjID());
		}
	}
	if (pTree->SetTargetObjId(setTargets)) {
		SNK_LOGGER_ASYNC_DEBUG("[行为树相关]location目标选择成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature);
		return E_NODE_SUCCESS;
	}
	return E_NODE_FAILURE;
}
