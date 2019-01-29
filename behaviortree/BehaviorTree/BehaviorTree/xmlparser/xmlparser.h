#pragma once
#pragma execution_character_set("utf-8")
//强制无签名utf-8
#ifndef _XML_PARSER_H_
#define _XML_PARSER_H_

#include "3rdparty/tinyxml2.h"
#include "include/tree_builder.h"

using namespace tinyxml2;
using namespace BT;
class CXMLParser
{
public:
	CXMLParser():m_oXML(){}
	~CXMLParser() = default;
	bool Init(CBehaviorTreeBuilder* pBt) {
		if (pBt) {
			m_pBt = pBt;
			return true;
		}
		return false;
	}
	bool LoadAndBuildTree(const char* pszFile);
	bool Travel(XMLElement* pNode);
protected:
	EOperator __GetOperator(const char* pOper) {
		if (pOper) {
			if (__StrEqual(pOper, "==")) {
				return E_OPERATOR_EQ;
			}
			else if (__StrEqual(pOper, "!="))
			{
				return E_OPERATOR_NE;
			}
			else if (__StrEqual(pOper, ">")) {
				return E_OPERATOR_GT;
			}
			else if (__StrEqual(pOper, ">=")) {
				return E_OPERATOR_GE;
			}
			else if (__StrEqual(pOper, "<")) {
				return E_OPERATOR_LT;
			}
			else if (__StrEqual(pOper, "<=")) {
				return E_OPERATOR_LE;
			}
		}
		return E_OPERATOR_ERR;
	}
	inline bool __StrEqual(const char* str1, const char* str2) {
		return strcmp(str1, str2) == 0;
	}
	void __DealCondition(XMLElement* pNode);
	void __DealAction(XMLElement *pNode);
private:
	XMLDocument m_oXML;
	CBehaviorTreeBuilder* m_pBt;
};
#endif