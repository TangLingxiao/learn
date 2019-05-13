
#ifndef _XML_PARSER_H_
#define _XML_PARSER_H_

#include "../../lib3rd/sgdp/src/tinyxml2/tinyxml2.h"
#include "../nodes/basic_types.h"
#include "../nodes/tree_node.h"
using namespace tinyxml2;
namespace BT
{
	class CBehaviorTreeMgr;
	//根据xml配置构造行为树
	class CXMLParser
	{
	public:
		CXMLParser() :m_oXML() {}
		~CXMLParser() = default;
		bool Init(CBehaviorTreeMgr* pBt) {
			if (pBt) {
				m_pBt = pBt;
				return true;
			}
			return false;
		}

		//加载xml
		bool LoadAndBuildTree(const char* pszFile);

		//遍历xml构造行为树
		bool Travel(XMLElement* pNode);
	private:
		XMLDocument m_oXML;
		CBehaviorTreeMgr* m_pBt = nullptr;
		const char* m_pszFile = nullptr;
	};
}
#endif