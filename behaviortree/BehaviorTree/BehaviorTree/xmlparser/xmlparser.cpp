#include "xmlparser.h"
#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8

bool CXMLParser::LoadAndBuildTree(const char* pszFile)
{
	if (NULL == pszFile)
		return false;
	if (XML_SUCCESS != m_oXML.LoadFile(pszFile))
		return false;
	XMLElement* pRootElement = m_oXML.RootElement();
	return Travel(pRootElement);
}

bool CXMLParser::Travel(XMLElement* pNode)
{
	if (!pNode)
		return false;
	while (pNode) {
		if (__StrEqual(pNode->Name() ,"action")) {
			__DealAction(pNode);
		}
		else if (__StrEqual(pNode->Name(), "condition")) {
			__DealCondition(pNode);	
		}
		else if (__StrEqual(pNode->Name(), "sequence")) {
			CTreeNode* pSeq = m_pBt->CreateSequence();
			m_pBt->AddTreeNode(pSeq);
			XMLElement* pFirst = pNode->FirstChildElement();
			bool bRet = Travel(pFirst);
			if (!bRet) {
				return false;
			}
			m_pBt->Back();
		}
		else if (__StrEqual(pNode->Name() ,"loop")) {
			CTreeNode* pSeq = m_pBt->CreateLoop(0);
			m_pBt->AddTreeNode(pSeq);
			XMLElement* pFirst = pNode->FirstChildElement();
			bool bRet = Travel(pFirst);
			if (!bRet) {
				return false;
			}
			m_pBt->Back();
		}
		else if (__StrEqual(pNode->Name(), "select")) {
			CTreeNode* pSeq = m_pBt->CreateSelect();
			m_pBt->AddTreeNode(pSeq);
			XMLElement* pFirst = pNode->FirstChildElement();
			bool bRet = Travel(pFirst);
			if (!bRet) {
				return false;
			}
			m_pBt->Back();
		}
		pNode = pNode->NextSiblingElement();
	}
	return true;
}

void CXMLParser::__DealCondition(XMLElement * pNode)
{
	if (!pNode)
		return;
	const char* pType = pNode->Attribute("type");
	if (pType) {
		CTreeNode* pAction = NULL;
		if (__StrEqual(pType, "rage")) {
			const char* pCamp = pNode->Attribute("camp");
			const char* pOper = pNode->Attribute("operator");
			const char* pNum = pNode->Attribute("num");
			EConditionCamp eCamp = E_CAMP_OTHER;
			EOperator eOperator = E_OPERATOR_ERR;
			int nNum = 0;
			if (pCamp) {
				eCamp = (EConditionCamp)atoi(pCamp);
			}
			eOperator = __GetOperator(pOper);
			if (eOperator == E_OPERATOR_ERR) {
				throw("error operatro");
			}
			if (pNum) {
				nNum = atoi(pNum);
			}
			pAction = m_pBt->CreateRageCondition(nNum, eCamp, eOperator);
		}
		else if (__StrEqual(pType, "hp")) {
			const char* pCamp = pNode->Attribute("camp");
			const char* pOper = pNode->Attribute("operator");
			const char* pNum = pNode->Attribute("num");
			const char* pParam = pNode->Attribute("param");
			const char* pSelf = pNode->Attribute("self");
			EConditionCamp eCamp = E_CAMP_OTHER;
			EOperator eOperator = E_OPERATOR_ERR;
			EHPCondition eParam = E_HP_NUM;
			bool bSelf = false;
			int nNum = 0;
			if (pCamp){
				eCamp = (EConditionCamp)atoi(pCamp);
			}
			eOperator = __GetOperator(pOper);
			if (eOperator == E_OPERATOR_ERR) {
				throw("error operatro");
			}
			if (pNum) {
				nNum = atoi(pNum);
			}
			if (pParam) {
				if (__StrEqual(pParam, "percent")) {
					eParam = E_HP_PERCENT;
				}
				else if (__StrEqual(pParam, "num")) {
					eParam = E_HP_NUM;
				}
				else {
					throw("error param");
				}
			}
			if (pSelf) {
				if (atoi(pSelf) != 0) {
					bSelf = true;
				}
			}
			pAction = m_pBt->CreateHpCondition(nNum, eCamp, eOperator, eParam, bSelf);
		}
		else if (__StrEqual(pType, "attr")) {
			const char* pHave = pNode->Attribute("have");
			const char* pId = pNode->Attribute("attrid");
			bool bHave = false;
			int nAttrId = 0;
			if (pHave) {
				if (atoi(pHave) != 0) {
					bHave = true;
				}
			}
			if (pId) {
				nAttrId = atoi(pId);
			}
			pAction = m_pBt->CreateAttrCondition(nAttrId, bHave);
		}
		else if (__StrEqual(pType, "energy")) {
			const char* pOperator = pNode->Attribute("operator");
			const char* pNum = pNode->Attribute("num");
			EOperator eOperator = E_OPERATOR_ERR;
			int nNum = 0;
			eOperator = __GetOperator(pOperator);
			if (eOperator == E_OPERATOR_ERR) {
				throw("error operator");
			}
			if (pNum) {
				nNum = atoi(pNum);
			}
			pAction = m_pBt->CreateEnergyCondition(nNum, eOperator);
		}
		else if (__StrEqual(pType, "numeric")) {
			const char* pCamp = pNode->Attribute("camp");
			const char* pOper = pNode->Attribute("operator");
			const char* pNum = pNode->Attribute("num");
			const char* pParam = pNode->Attribute("param");
			const char* pSelf = pNode->Attribute("self");
			EConditionCamp eCamp = E_CAMP_OTHER;
			EOperator eOperator = E_OPERATOR_ERR;
			ENumericCondition eParam = E_NUMERIC_ATTACK;
			bool bSelf = false;
			int nNum = 0;
			if (pCamp) {
				eCamp = (EConditionCamp)atoi(pCamp);
			}
			eOperator = __GetOperator(pOper);
			if (eOperator == E_OPERATOR_ERR) {
				throw("error operatro");
			}
			if (pNum) {
				nNum = atoi(pNum);
			}
			if (pParam) {
				if (__StrEqual(pParam, "attack")) {
					eParam = E_NUMERIC_ATTACK;
				}
				else if (__StrEqual(pParam, "def")) {
					eParam = E_NUMERIC_DEF;
				}
				else if (__StrEqual(pParam, "cri")) {
					eParam = E_NUMERIC_CRI;
				}
				else if (__StrEqual(pParam, "domage")) {
					eParam = E_NUMERIC_DOMAGE;
				}
				else if (__StrEqual(pParam, "speed")) {
					eParam = E_NUMERIC_SPEED;
				}
				else {
					throw("error param");
				}
			}
			if (pSelf) {
				if (atoi(pSelf) != 0) {
					bSelf = true;
				}
			}
			pAction = m_pBt->CreateNumericCondition(nNum, eCamp, eOperator, eParam, bSelf);
		}
		else if (__StrEqual(pType, "prob")) {
			const char* pNum = pNode->Attribute("num");
			int nNum = 0;
			if (pNum) {
				nNum = atoi(pNum);
			}
			pAction = m_pBt->CreateProbCondition(nNum);
		}
		if (pAction) {
			m_pBt->AddTreeNode(pAction);
			m_pBt->Back();
		}
	}
}

void CXMLParser::__DealAction(XMLElement * pNode)
{
	if (!pNode)
		return;
	const char* pType = pNode->Attribute("type");
	if (pType) {
		CTreeNode* pAction = NULL;
		if (__StrEqual(pType, "select"))
		{
			const char* pSubType = pNode->Attribute("subtype");
			if (pSubType)
			{
				if (__StrEqual(pSubType, "hit")) {
					pAction = m_pBt->CreateHitAction();
				}
				else if (__StrEqual(pSubType, "skill")) {
					const char* pSkillId = pNode->Attribute("skillid");
					int nSkillId = 0;
					if (pSkillId) {
						nSkillId = atoi(pSkillId);
					}
					pAction = m_pBt->CreateSkillAction(nSkillId);
				}
				else if (__StrEqual(pSubType, "super")) {
					pAction = m_pBt->CreateSuperAction();
				}
				else if (__StrEqual(pSubType, "change")) {
					pAction = m_pBt->CreateChangeAction();
				}
				else if (__StrEqual(pSubType, "teamskill")) {
					pAction = m_pBt->CreateTeamSkillAction();
				}
				else if (__StrEqual(pSubType, "other")) {
					pAction = m_pBt->CreateOtherAction();
				}
			}
		}
		else if (__StrEqual(pType, "exec"))
		{
			pAction = m_pBt->CreateExecAction();
		}
		else if (__StrEqual(pType, "safe")) {
			pAction = m_pBt->CreateSafeAction();
		}
		if (pAction) {
			m_pBt->AddTreeNode(pAction);
			m_pBt->Back();
		}
	}
}
