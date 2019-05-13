#include "action_node_impl.h"
#include "basic_funcs.h"
#include "utility.h"
#include "configmgr.h"
BT::ENodeStatus BT::CSelectHitAction::tick(CBehaviorTree* pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	CSkill * pSkill = pCurCreature->GetGroupIdSkill(E_SKILL_GROUP_NORMAL, true, false);
	if (!pSkill) {
		return E_NODE_FAILURE;
	}

	const CfgSkill * pCfg = pSkill->GetTemplate();
	if (pCfg == NULL) {
		return E_NODE_FAILURE;
	}

	pTree->SetSkillType(pSkill->GetType(pCfg));
	pTree->SetSkillCfg(pCfg);
	pTree->SetSelectAction(E_SELECT_HIT);
	SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]动作选择成功：普攻, 行为树id: %d,节点名字: %s,skillid: %d", pTree->GetTreeID(), this->GetNodeName(), pSkill->GetGroup());
	return E_NODE_SUCCESS;
}

bool BT::CSelectSkillAction::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pSkillId = pNode->Attribute("skillid");
	if (!pSkillId) {
		return false;
	}
	m_nSkillId = atoi(pSkillId);
	return true;
}

BT::ENodeStatus BT::CSelectSkillAction::tick(CBehaviorTree* pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	CSkill * pSkill = pCurCreature->GetGroupIdSkill(E_SKILL_GROUP_SKILL, true, false);
	if (!pSkill) {
		return E_NODE_FAILURE;
	}

	const CfgSkill * pCfg = pSkill->GetTemplate();
	if (pCfg == NULL) {
		return E_NODE_FAILURE;
	}

	pTree->SetSkillType(pSkill->GetType(pCfg));
	pTree->SetSkillCfg(pCfg);
	pTree->SetSelectAction(E_SELECT_SKILL);
	SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]动作选择成功：技能, 行为树id: %d,节点名字: %s,skillid: %d", pTree->GetTreeID(), this->GetNodeName(), pSkill->GetGroup());
	return E_NODE_SUCCESS; 
}

BT::ENodeStatus BT::CSelectSuperAction::tick(CBehaviorTree* pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	CSkill * pSkill = pCurCreature->GetGroupIdSkill(E_SKILL_GROUP_ULTIMATE, true, false);
	if (!pSkill) {
		return E_NODE_FAILURE;
	}

	const CfgSkill * pCfg = pSkill->GetTemplate();
	if (pCfg == NULL) {
		return E_NODE_FAILURE;
	}

	pTree->SetSkillType(pSkill->GetType(pCfg));
	pTree->SetSkillCfg(pCfg);
	pTree->SetSelectAction(E_SELECT_SUPER);
	SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]动作选择成功：大招, 行为树id: %d,节点名字: %s,skillid: %d", pTree->GetTreeID(), this->GetNodeName(), pSkill->GetGroup());
	return E_NODE_SUCCESS;
}

bool BT::CSelectExplosionAction::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pSkillId = pNode->Attribute("skillid");
	if (pSkillId) {
		m_nSkillGroupId = atoi(pSkillId);
	}
	return true;
}

BT::ENodeStatus BT::CSelectExplosionAction::tick(CBehaviorTree * pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	
	//已经处于爆气状态了
	if (pCurCreature->HasBuffType(E_EFFECT_TYPE_EXPLOSION)){
		return E_NODE_FAILURE;
	}
	//找到爆气技能
	CSkill * pExpSkill = pCurCreature->GetGroupIdSkill(E_SKILL_GROUP_TBD_ONE, true, false);
	if (!pExpSkill) {
		return E_NODE_FAILURE;
	}
	std::vector<const CfgSkill* > vecSkills;
	int nSkillId = 0;
	int nGroup = 0;
	const CfgSkill * pCfg = nullptr;
	//找爆气后技能
	if (m_nSkillGroupId != 0)
	{
		auto pSkill = pCurCreature->GetGroupIdSkill(m_nSkillGroupId, true, true);
		if (!pSkill)
		{
			return E_NODE_FAILURE;
		}

		pCfg = pSkill->GetTemplate();
		if (!pCfg)
		{
			return E_NODE_FAILURE;
		}

		vecSkills.push_back(pCfg);
	}
	else
	{
		CSkill* pSkill = nullptr;
		auto FindSkill = [&](ESkillGroup eGroup) {
			pSkill = pCurCreature->GetGroupIdSkill(eGroup, true, true);
			if (pSkill)
			{
				pCfg = pSkill->GetTemplate();
				if (pCfg)
				{
					vecSkills.push_back(pCfg);
				}
			}
		};
		FindSkill(E_SKILL_GROUP_NORMAL);
		FindSkill(E_SKILL_GROUP_SKILL);
		FindSkill(E_SKILL_GROUP_ULTIMATE);
		
	}

	
	if (!vecSkills.empty())
	{
		int nIndex = GNDP::GetRand(0, (int)vecSkills.size() - 1);
		pCfg = vecSkills[nIndex];
		nSkillId = pCfg->m_nType;
		nGroup = pCfg->m_nSkillGroup;
	}
	if (nSkillId != 0 && pCfg) {
		pTree->SetSkillType(nSkillId);
		pTree->SetExpSkillType(pExpSkill->GetType());
		pTree->SetSkillCfg(pCfg);
		pTree->SetSelectAction(E_SELECT_EXPLOSION);
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]动作选择成功：爆气, 行为树id: %d,节点名字: %s,skillid: %d", pTree->GetTreeID(), this->GetNodeName(), nGroup);
		return E_NODE_SUCCESS;

	}	
	return E_NODE_FAILURE;
}

bool BT::CSelectChangeAction::LoadXML(XMLElement * pNode)
{
	if (!pNode) {
		return false;
	}
	const char* pSkillId = pNode->Attribute("skillid");
	if (pSkillId) {
		m_nHeroId = atoi(pSkillId);
	}
	return true;
}

bool BT::CSelectChangeAction::CheckCanChange(CBehaviorTree* pTree, int &nAssitPos, int &nAssistHeroId)
{
	if (!pTree) {
		return false;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature || pCurCreature->IsDead()) {
		return false;
	}
	IBattle *pBattle = pCurCreature->GetBattle();
	if (!pBattle) {
		return false;
	}
	CBattleRole* pRole = pCurCreature->GetOwner();
	if (!pRole) {
		return false;
	}

	
#pragma region 
	//
	const CfgSkill * pCfg = nullptr;
	int nPos = 0;
	std::vector<std::tuple<int, int, const CfgSkill * > > vecAssit;
	for (; nPos < MAX_FORMATION_SUPPORT; ++nPos) {
		CCreature* pCreature = pRole->FindAssistCreatureByPos(nPos);
		if (!pCreature || pCreature->IsDead() || pCurCreature == pCreature) {
			continue;
		}
		if (m_nHeroId != 0 && m_nHeroId != pCreature->GetTemplateId()) {
			continue;
		}
		if (pCurCreature->GetAssistFlag() || pCurCreature->HasBuffType(E_EFFECT_TYPE_CHARGE_ATTACK))
		{
			continue;
		}
		if (1 == pRole->m_nAssistFlag[nPos]) {
			continue;
		}
		int32 nAssistSkillID = pCreature->GetAssistSkill();
		if (0 == nAssistSkillID) {
			continue;
		}
		pCfg = _SNK_CAllConfig()->GetSkillConfig().FindSkill(nAssistSkillID, 1);
		if (!pCfg) {
			continue;
		}
		if (pBattle->CheckCanExchangeFightHero(pRole, pCurCreature->GetPos(), nPos)) {
			vecAssit.push_back(std::make_tuple(nPos, pCreature->GetTemplateId(), pCfg));
		}
	}
	if (vecAssit.empty()) {
		return false;
	}

	int nRand = GetRand(0, (int)vecAssit.size() - 1);
	nAssitPos = std::get<0>(vecAssit[nRand]);
	nAssistHeroId = std::get<1>(vecAssit[nRand]);
	pCfg = std::get<2>(vecAssit[nRand]);
	pTree->SetSkillType(pCfg->m_nType);
	pTree->SetSkillCfg(pCfg);
#pragma endregion

#if VERSION_CE
	//如果上一个行为有蓄力效果则不能使用援护
	const std::array<ESelectAction, 2>& aSelect = pTree->GetAllSelectAction();
	if (aSelect[0] != E_SELECT_NONE) {
		CSkill* pSkill = pCurCreature->FindSkill(pTree->GetSkillType());
		if (pSkill == NULL) {
			return false;
		}
		CEffect* pEffect = pSkill->GetEffect();
		if (pEffect == NULL) {
			return false;
		}
		const CfgSkillEffect * pCfg = pEffect->GetTemplate();
		if (pCfg == NULL) {
			return false;
		}
		if (pCfg->m_nBuffType == E_EFFECT_TYPE_CHARGE_ATTACK) {
			return false;
		}
	}

	bool bCanChange = false;// pBattle->CheckCanExchangeFightHero(pRole, nPos, E_ONEXCHANGE_HERO_CHOSE, false, false);
#endif
	return true;
}


BT::ENodeStatus BT::CSelectChangeAction::tick(CBehaviorTree* pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	int nAssitHeroId = 0;
	int nAssitPos = 0;
	if (!CheckCanChange(pTree, nAssitPos, nAssitHeroId)) {
		return E_NODE_FAILURE;
	}
	pTree->SetAssistPos(nAssitPos);
	pTree->SetSelectAction(E_SELECT_CHANGE);
	//if (m_nHeroId) {
	//	assert(m_nHeroId == nAssitHeroId);
	//}
	SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]动作选择成功：援护, 援护id:%d, 行为树id: %d,节点名字: %s", nAssitHeroId, pTree->GetTreeID(), this->GetNodeName());
	return E_NODE_SUCCESS;
}

BT::ENodeStatus BT::CSelectTeamSkillAction::tick(CBehaviorTree* pTree)
{
	if (!pTree) {
		return E_NODE_FAILURE;
	}
	CCreature* pCurCreature = pTree->GetTreeOwner();
	if (!pCurCreature) {
		return E_NODE_FAILURE;
	}
	CSkill * pSkill = pCurCreature->GetGroupIdSkill(E_SKILL_GROUP_ASSIST, true, false);
	if (!pSkill) {
		return E_NODE_FAILURE;
	}

	const CfgSkill * pCfg = pSkill->GetTemplate();
	if (pCfg == NULL) {
		return E_NODE_FAILURE;
	}

	pTree->SetSkillType(pSkill->GetType(pCfg));
	pTree->SetSkillCfg(pCfg);
	pTree->SetSelectAction(E_SELECT_TEAMSKILL);
	SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]动作选择成功：连携, 行为树id: %d,节点名字: %s,skillid: %d", pTree->GetTreeID(), this->GetNodeName(), pSkill->GetGroup());
	return E_NODE_SUCCESS;
}

BT::ENodeStatus BT::CSelectOtherAction::tick(CBehaviorTree* pTree)
{
	//暂时不做
	//if (!pTree) {
	//	return E_NODE_FAILURE;
	//}
	//CCreature* pCurCreature = pTree->GetTreeOwner();
	//if (!pCurCreature) {
	//	return E_NODE_FAILURE;
	//}
	//pTree->SetSelectAction(E_SELECT_OTHER);
	return E_NODE_FAILURE;
}

BT::ENodeStatus BT::CExecAction::tick(CBehaviorTree* pTree)
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
	CBattleRole* pRole = pCurCreature->GetOwner();
	if (!pRole) {
		return E_NODE_FAILURE;
	}
	int nRet = -1;
	int nTargetId = -1;
	nTargetId = pTree->GetTargetObjId();
	if (-1 == nTargetId) {
		return E_NODE_FAILURE;
	}
#if VERSION_CE
	const auto &aSelectAction = pTree->GetAllSelectAction();
	for (const auto& eSelectAction : aSelectAction) {
#else
	auto eSelectAction = pTree->GetSelectAction();
#endif
		switch (eSelectAction)
		{
		case BT::E_SELECT_HIT:
			/*fall through*/
		case BT::E_SELECT_SKILL:
			/*fall through*/
		case BT::E_SELECT_SUPER:
			/*fall through*/
		case BT::E_SELECT_TEAMSKILL:
			nRet = BTUseSkill(pCurCreature, pTree->GetSkillType(), nTargetId);
			break;
		case BT::E_SELECT_CHANGE:
			pBattle->HandleExchangeHero(pRole->m_ullPlayerId, pTree->GetAssistPos(), nTargetId);
			SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]援护换人, 行为树id: %d,节点名字: %s, creatureid: %d, creature: %p, targetobjid: %d", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature, nTargetId);
			nRet = 0;
			break;
		case BT::E_SELECT_EXPLOSION:
			//先对自己放一个爆气技能
			BTUseSkill(pCurCreature, pTree->GetExpSkillType(), pCurCreature->GetObjID());
			SNK_LOGGER_ASYNC_DEBUG("[行为树相关]执行爆气, 行为树id: %d,节点名字: %s, creatureid: %d, creature: %p, targetobjid: %d", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature, pCurCreature->GetObjID());
			nRet = BTUseSkill(pCurCreature, pTree->GetSkillType(), nTargetId);
			break;
		case BT::E_SELECT_NONE:
			break;
		case BT::E_SELECT_OTHER:
			break;
		default:
			break;
		}
#if VERSION_CE
		if (nRet != 0) {
			break;
		}
	}
#endif
	int nSkillId = pTree->GetSkillType();
	if (nRet == 0) {
		pTree->ClearTree();
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]动作执行成功, 行为树id: %d,节点名字: %s, creatureid: %d, creature: %p, targetobjid: %d, skillid: %d", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature, nTargetId, nSkillId);
		return E_NODE_SUCCESS;
	}
	SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]动作执行失败, 失败原因: %d, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p, targetobjid: %d, skillid: %d", nRet, pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature, nTargetId, nSkillId);
	return E_NODE_FAILURE;
}

BT::ENodeStatus BT::CSafeAction::tick(CBehaviorTree* pTree)
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

	int nSkillId = -1;
	const CfgSkill* pCfg = nullptr;
	const std::map<uint32, CSkill*>& mapSkills = pCurCreature->GetSkill();
	for (const auto& it : mapSkills) {
		CSkill* pSkill = it.second;
		if (!pSkill) {
			continue;
		}
		if (pSkill->GetGroup() == E_SKILL_GROUP_NORMAL) {
			if (pCurCreature->HasBuffType(E_EFFECT_TYPE_EXPLOSION) && pSkill->GetTemplate()->m_nPreImproveID == 0)
			{
				continue;
			}
			int nRet = pSkill->CheckUse(*pCurCreature, E_SKILL_LAUNCH_TYPE_NORMAL);
			if (nRet != 0) {
				continue;
			}
			pCfg = pSkill->GetTemplate();
			nSkillId = pSkill->GetType(pCfg);
			break;
		}
	}
	if (nSkillId == -1 || pCfg == nullptr) {
		game_error_ntf oNtf;
		oNtf.set_error_num(RES_ERROR_None);
		std::string error_str = "技能配置错误，没有普攻, heroid:" + GNDP::ToString(pCurCreature->GetTemplateId());
		oNtf.set_error_str(error_str);
		pBattle->BroadcastMsg(SProtoSpace::game_error_ntf_id, oNtf);
		SNK_LOGGER_ASYNC_CRITICAL( "[行为树相关]执行安全节点失败, 技能配置错误，找不到普攻, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature);
		return E_NODE_FAILURE;
	}
	std::vector<CCreature*> vecTemp;
	int nRet = pBattle->OptionalTargetGet(*pCurCreature, vecTemp, pCfg);
	if (nRet != 0) {
		game_error_ntf oNtf;
		oNtf.set_error_num(RES_ERROR_None);
		std::string error_str = "技能配置错误，普攻目标选择配置错误, heroid:" + GNDP::ToString(pCurCreature->GetTemplateId());
		oNtf.set_error_str(error_str);
		pBattle->BroadcastMsg(SProtoSpace::game_error_ntf_id, oNtf);
		SNK_LOGGER_ASYNC_CRITICAL( "[行为树相关]执行安全节点失败, 普攻目标选择配置错误, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p， camp: %d, pos: %d",\
			pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature, (ETargetCamp)pCfg->m_oTargetType.camp, (ETargetPosition)pCfg->m_oTargetType.pos);
		return E_NODE_FAILURE;
	}
	CBattleRole* pTargetRole = GetTargetRole(pCurCreature, E_CAMP_OTHER);
	if (!pTargetRole) {
		return E_NODE_FAILURE;
	}
	int nTargetObjId = -1;
	for (const auto& pCreature : vecTemp)
	{
		if (!pCreature || pCreature->IsDead()) {
			continue;
		}
		nTargetObjId = pCreature->GetObjID();
		break;
	}
	if (-1 == nTargetObjId) {
		game_error_ntf oNtf;
		oNtf.set_error_num(RES_ERROR_None);
		oNtf.set_error_str("没有可选目标");
		pBattle->BroadcastMsg(SProtoSpace::game_error_ntf_id, oNtf);
		SNK_LOGGER_ASYNC_CRITICAL( "[行为树相关]执行安全节点失败, 没有可选目标, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature);
		return E_NODE_FAILURE;
	}

	nRet = BTUseSkill(pCurCreature, nSkillId, nTargetObjId);
	pTree->ClearTree();
	if (nRet == 0) {
		SNK_LOGGER_ASYNC_DEBUG( "[行为树相关]执行安全节点成功, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p, targetobjid: %d, skillid: %d", pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature, nTargetObjId, nSkillId);
		return E_NODE_SUCCESS;
	}
	game_error_ntf oNtf;
	oNtf.set_error_num(RES_ERROR_None);
	std::string error_str = "释放普攻失败, 错误码:" + GNDP::ToString(nRet);
	oNtf.set_error_str(error_str);
	pBattle->BroadcastMsg(SProtoSpace::game_error_ntf_id, oNtf);
	SNK_LOGGER_ASYNC_CRITICAL( "[行为树相关]执行安全节点失败, 失败原因: %d, 行为树id: %d,节点名字: %s,creatureid: %d, creature: %p, targetobjid: %d, skillid: %d", nRet, pTree->GetTreeID(), this->GetNodeName(), pCurCreature->GetTemplateId(), pCurCreature, nTargetObjId, nSkillId);
	return E_NODE_FAILURE;
}

