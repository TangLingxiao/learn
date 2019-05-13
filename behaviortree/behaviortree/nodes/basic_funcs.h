#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__
#include "creature.h"
#include "battlerole.h"
#include "battle.h"
#include "skill.h"
#include "basic_types.h"
#include <algorithm>
#if defined(WIN32) || defined(WIN64)
#include <direct.h> 
#include <io.h>
#else
#include <dirent.h>
#include <unistd.h>
#include <regex>
#endif
namespace BT
{
	static void GetFiles(const std::string& strPath, std::vector<std::string>& vecFiles)
	{
#if defined(WIN32) || defined(WIN64)
		_finddata_t file;
		std::string strFilter = strPath + "/*xml";
		auto lf = _findfirst(strFilter.c_str(), &file);
		if (lf != -1)
		{
			do {
				vecFiles.push_back(strPath + "/" + file.name);
			} while (_findnext(lf, &file) == 0);
		}
		
		_findclose(lf);
#else
		auto dir = opendir(strPath.data());
		if (dir) {
			struct dirent* ent;
			std::regex reg_obj("(.*)(xml)", std::regex::icase);
			while ((ent = readdir(dir)) != NULL) {
				if (std::regex_match(ent->d_name, reg_obj)) {
					vecFiles.push_back(strPath + "/" + ent->d_name);
				}
			}
			closedir(dir);
		}
		
#endif
	}

	static int BTUseSkill(CCreature* pCreature, uint32 nSkillId, uint32 nTargetid)
	{
		if (!pCreature) {
			return false;
		}
		CBattleRole* pRole = pCreature->GetOwner();
		IBattle* pBattle = pCreature->GetBattle();
		if (!pRole || !pBattle) {
			return false;
		}
		uint64 nPlayerId = pRole->m_ullPlayerId;
		use_skill_req oReq;
		oReq.set_hero_objid(pCreature->GetObjID());
		oReq.set_skill_id(nSkillId);
		oReq.add_target_objid(nTargetid);
		oReq.set_launch_type(E_SKILL_LAUNCH_TYPE_NORMAL);
		return pBattle->UseSkillReq(nPlayerId, oReq);
	}

	static CBattleRole* GetTargetRole(CCreature* pCurCreature, ECampCondition eCamp)
	{
		if (!pCurCreature) {
			return nullptr;
		}
		CBattleRole* pTargetRole = nullptr;
		if (eCamp == E_CAMP_SELF) {
			pTargetRole = pCurCreature->GetOwner();
		}
		else {
			CBattleRole* pCurrole = pCurCreature->GetOwner();
			if (!pCurrole) {
				return nullptr;
			}
			IBattle* pBattle = pCurCreature->GetBattle();
			if (!pBattle) {
				return nullptr;
			}
			pTargetRole = pBattle->GetOther(pCurrole->m_ullPlayerId);
		}
		return pTargetRole;
	}

	static bool CheckResult(EOperator eOper, int left, int right)
	{
		switch (eOper)
		{
		case BT::E_OPERATOR_ERR:
			break;
		case BT::E_OPERATOR_LT:
			return left < right;
			break;
		case BT::E_OPERATOR_LE:
			return left <= right;
			break;
		case BT::E_OPERATOR_EQ:
			return left == right;
			break;
		case BT::E_OPERATOR_GT:
			return left > right;
			break;
		case BT::E_OPERATOR_GE:
			return left >= right;
			break;
		case BT::E_OPERATOR_NE:
			return left != right;
			break;
		default:
			break;
		}
		return false;
	}

	static int GetNumeric(CCreature* pCreature, ENumericCondition eNumeric)
	{
		if (!pCreature) {
			return -1;
		}
		switch (eNumeric)
		{
		case BT::E_NUMERIC_ATTACK:
			return pCreature->GetBattle_Atk(0, 0);
			break;
		case BT::E_NUMERIC_DEF:
			return  pCreature->GetBattle_Def(0, 0);
			break;
		case BT::E_NUMERIC_CRI:
			return pCreature->GetBattle_Crit(0);
			break;
		case BT::E_NUMERIC_DOMAGE:
			return pCreature->GetBattle_GetBlock(0);
			break;
		case BT::E_NUMERIC_SPEED:
			return pCreature->GetBattle_Speed();
			break;
		default:
			break;
		}
		return -1;
	}

	static int GetTarget(std::vector<std::pair<int, int> > &vecTargets, ERangeCondition eRange)
	{
		if (vecTargets.empty()) {
			return -1;
		}
		std::random_shuffle(vecTargets.begin(), vecTargets.end());
		if (eRange == E_RANGE_MIN) {
			std::sort(vecTargets.begin(), vecTargets.end(),
				[](std::pair<int, int> &lh, std::pair<int, int>& rh) -> bool {
				return lh.second < rh.second;
			});
		}
		else if (eRange == E_RANGE_MAX) {
			std::sort(vecTargets.begin(), vecTargets.end(),
				[](std::pair<int, int> &lh, std::pair<int, int>& rh) -> bool {
				return lh.second > rh.second;
			});
		}
		else
		{
			return -1;
		}
		return vecTargets.front().first;
	}
	//比较字符串是否相同
	inline bool StrEqual(const char* str1, const char* str2) {
		return strcmp(str1, str2) == 0;
	}

	//返回对应的运算符枚举
	static EOperator GetOperator(const char* pOper) {
		if (pOper) {
			if (StrEqual(pOper, "==")) {
				return E_OPERATOR_EQ;
			}
			else if (StrEqual(pOper, "!="))
			{
				return E_OPERATOR_NE;
			}
			else if (StrEqual(pOper, ">")) {
				return E_OPERATOR_GT;
			}
			else if (StrEqual(pOper, ">=")) {
				return E_OPERATOR_GE;
			}
			else if (StrEqual(pOper, "<")) {
				return E_OPERATOR_LT;
			}
			else if (StrEqual(pOper, "<=")) {
				return E_OPERATOR_LE;
			}
		}
		return E_OPERATOR_ERR;
	}


}
#endif

