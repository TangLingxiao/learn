#include "xmlparser.h"
#include "treemgr/tree_mgr.h"
#include "nodes/basic_funcs.h"

using namespace BT;
bool CXMLParser::LoadAndBuildTree(const char* pszFile)
{
	if (NULL == pszFile)
		return false;
	m_pszFile = pszFile;
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
		if (StrEqual(pNode->Name(), "action") || StrEqual(pNode->Name(), "condition")) {
			const char* pType = pNode->Attribute("type");
			const char* pName = pNode->Attribute("name");
			if (!pName) {
				SNK_LOGGER_ASYNC_CRITICAL( "[行为树相关]load node error, filename: %s, line:%d", m_pszFile, pNode->GetLineNum());
				return false;
			}
			if (!pType) {
				SNK_LOGGER_ASYNC_CRITICAL( "[行为树相关]load node error, filename: %s, line:%d", m_pszFile, pNode->GetLineNum());
				return false;
			}
			ITreeNode* pAction = m_pBt->CreateNode(pType);
			if (!pAction) {
				SNK_LOGGER_ASYNC_CRITICAL( "[行为树相关]load node error, filename: %s, line:%d", m_pszFile, pNode->GetLineNum());
				return false;
			}

			pAction->SetNodeName(pName);
			if (!pAction->LoadXML(pNode)) {
				SNK_LOGGER_ASYNC_CRITICAL( "[行为树相关]load node error, filename: %s, line:%d", m_pszFile, pNode->GetLineNum());
				return false;
			}
			m_pBt->AddTreeNode(pAction);
			m_pBt->Back();

		}
		else if (StrEqual(pNode->Name(), "sequence") || StrEqual(pNode->Name(), "select")) {
			const char* pName = pNode->Attribute("name");
			ITreeNode* pSeq = m_pBt->CreateNode(pNode->Name());
			if (!pSeq) {
				return false;
			}
			pSeq->SetNodeName(pName);
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