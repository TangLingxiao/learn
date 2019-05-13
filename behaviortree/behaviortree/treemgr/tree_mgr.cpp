#include "tree_mgr.h"
#include "utility.h"
#include "nodes/basic_funcs.h"
using namespace BT;

#if defined(WIN32) || defined(WIN64)
#define AI_CONFIG "../../../config/aiconfig"
#else
#define AI_CONFIG "../../config/aiconfig"
#endif

IMPLEMENT_SINGLETON(CBehaviorTreeMgr);

uint32 BT::CBehaviorTreeMgr::_GetNextTreeID()
{
	m_nTreeID++;
	if (m_nTreeID == INVALID_32BIT_ID || 0 == m_nTreeID)
	{
		m_nTreeID++;
	}
	return m_nTreeID;
}

uint32 BT::CBehaviorTreeMgr::_GetNextNodeID()
{
	m_nNodeID++;
	if (m_nNodeID == INVALID_32BIT_ID || 0 == m_nNodeID)
	{
		m_nNodeID++;
	}
	return m_nNodeID;
}

BT::CBehaviorTreeMgr::CBehaviorTreeMgr()
{
	m_mapOldBehaviorTree.clear();
	m_mapOldTreeNode.clear();
	m_mapNewTreeNode.clear();
	m_mapNewBehaviorTree.clear();
}

BT::CBehaviorTreeMgr::~CBehaviorTreeMgr()
{
	ReleaseOldNode();
	ReleaseOldTree();
	ReleaseNewNode();
	ReleaseNewTree();
}

bool BT::CBehaviorTreeMgr::Init()
{
	m_oTreePool.Init(64, 16);

	//condition
	_node_factory.RegisterCteator("hp", new PoolMaker<CHPCondition>());
	_node_factory.RegisterCteator("rage", new PoolMaker<CRageCondition>());
	_node_factory.RegisterCteator("attr", new PoolMaker<CAttrCondition>());
	_node_factory.RegisterCteator("energy", new PoolMaker<CEnergyCondition>());
	_node_factory.RegisterCteator("numeric", new PoolMaker<CNumericCondition>());
	_node_factory.RegisterCteator("prob", new PoolMaker<CProbCondition>());
	_node_factory.RegisterCteator("round", new PoolMaker<CRoundCondition>());
	_node_factory.RegisterCteator("class", new PoolMaker<CClassCondition>());
	_node_factory.RegisterCteator("fighter", new PoolMaker<CFighterCondition>());
	_node_factory.RegisterCteator("team", new PoolMaker<CTeamCondition>());
	_node_factory.RegisterCteator("level", new PoolMaker<CLevelCondition>());
	_node_factory.RegisterCteator("turn", new PoolMaker<CTurnCondition>());
	_node_factory.RegisterCteator("state", new PoolMaker<CStateCondition>());

	//select_condition
	_node_factory.RegisterCteator("select_hp", new PoolMaker<CSelectHPCondition>());
	_node_factory.RegisterCteator("select_attr", new PoolMaker<CSelectAttrCondition>());
	_node_factory.RegisterCteator("select_rage", new PoolMaker<CSelectRageCondition>());
	_node_factory.RegisterCteator("select_numeric", new PoolMaker<CSelectNumericCondition>());
	_node_factory.RegisterCteator("select_fighter", new PoolMaker<CSelectFighterCondition>());
	_node_factory.RegisterCteator("select_level", new PoolMaker<CSelectLevelCondition>());
	_node_factory.RegisterCteator("select_state", new PoolMaker<CSelectStateCondition>());
	_node_factory.RegisterCteator("select_prob", new PoolMaker<CSelectProbCondition>());
	_node_factory.RegisterCteator("select_location", new PoolMaker<CSelectLocationCondition>());

	//action
	_node_factory.RegisterCteator("exec", new PoolMaker<CExecAction>());
	_node_factory.RegisterCteator("safe", new PoolMaker<CSafeAction>());
	_node_factory.RegisterCteator("hit", new PoolMaker<CSelectHitAction>());
	_node_factory.RegisterCteator("skill", new PoolMaker<CSelectSkillAction>());
	_node_factory.RegisterCteator("super", new PoolMaker<CSelectSuperAction>());
	_node_factory.RegisterCteator("change", new PoolMaker<CSelectChangeAction>());
	_node_factory.RegisterCteator("teamskill", new PoolMaker<CSelectTeamSkillAction>());
	_node_factory.RegisterCteator("other", new PoolMaker<CSelectOtherAction>());
	_node_factory.RegisterCteator("explosion", new PoolMaker<CSelectExplosionAction>());
	//logic
	_node_factory.RegisterCteator("sequence", new PoolMaker<CSequenceNode>());
	_node_factory.RegisterCteator("select", new PoolMaker<CSelectNode>());
	
	if (!m_oXmlParser.Init(this)) {
		return false;
	}
	return Load();
}

bool BT::CBehaviorTreeMgr::Load()
{
	char szFileExePath[MAX_PATH] = { 0 };
	const char *pszExePath = GNDP::GetExePath();
#if defined(WIN32) || defined(WIN64)
	snprintf(szFileExePath, sizeof(szFileExePath) - 1, "%s/%s", pszExePath, AI_CONFIG);
#else
	snprintf(szFileExePath, sizeof(szFileExePath) - 1, "%s/%s", pszExePath, AI_CONFIG);
#endif
	std::vector<std::string> vecFiles;
	GetFiles(szFileExePath, vecFiles);	
	for (const auto& strFild : vecFiles) {
		bool bRet = m_oXmlParser.LoadAndBuildTree(strFild.data());
		if (!bRet) {
			return false;
		}
		std::vector<std::string> vecSub;
		GNDP::StrSplit(strFild.c_str(), "/", vecSub);
		std::string filename = vecSub.back().substr(0, vecSub.back().find_last_of('.'));
		this->CreateTree(atoi(filename.c_str()));
	}
	return true;
}

bool BT::CBehaviorTreeMgr::Reload()
{
	//每次reload置反标志位
	m_bUseNew = !m_bUseNew;
	char szFileExePath[MAX_PATH] = { 0 };
	const char *pszExePath = GNDP::GetExePath();
#if defined(WIN32) || defined(WIN64)
	snprintf(szFileExePath, sizeof(szFileExePath) - 1, "%s/%s", pszExePath, AI_CONFIG);
#else
	snprintf(szFileExePath, sizeof(szFileExePath) - 1, "%s/%s", pszExePath, AI_CONFIG);
#endif
	std::vector<std::string> vecFiles;
	GetFiles(szFileExePath, vecFiles);
	for (const auto& strFile : vecFiles) {
		bool bRet = m_oXmlParser.LoadAndBuildTree(strFile.data());
		if (!bRet) {
			//标志位修改回原来
			m_bUseNew = !m_bUseNew;
			//清理修改失败的数据
			RecycleOldTree();
			SNK_LOGGER_ASYNC_CRITICAL( "[行为树相关]热加载失败，失败文件为:%s", strFile.data());
			return false;
		}
		std::vector<std::string> vecSub;
		GNDP::StrSplit(strFile.c_str(), "/", vecSub);
		std::string filename = vecSub.back().substr(0, vecSub.back().find_last_of('.'));
		this->CreateTree(atoi(filename.c_str()));
	}
	//清理旧数据
	RecycleOldTree();
	return true;
}


BT::CBehaviorTree* BT::CBehaviorTreeMgr::CreateTree(int nTreeId)
{
	while (!m_stTree.empty()) {
		m_stTree.pop();
	}
	CBehaviorTree *pTree = m_oTreePool.FetchObj();//new CBehaviorTree();
	if (!pTree) {
		m_pTreeRoot = NULL;
		return nullptr;
	}
	pTree->SetTreeID(nTreeId/*_GetNextTreeID()*/);
	if (m_bUseNew) {
		m_mapNewBehaviorTree[pTree->GetTreeID()] = pTree;
	}
	else{
		m_mapOldBehaviorTree[pTree->GetTreeID()] = pTree;
	}
	pTree->SetTreeRoot(m_pTreeRoot);
	m_pTreeRoot = NULL;
	return pTree;
}

void BT::CBehaviorTreeMgr::_ReleaseNodeObj(ITreeNode* pNode)
{
	if (!pNode) {
		return;
	}
	const char* pszType = pNode->GetStrNodeType();
	ProductMaker<ITreeNode>* pNodeMaker = _node_factory.GetCreator(pszType);
	if (pNodeMaker) {
		pNodeMaker->ReleaseObj(pNode);
	}
}

void BT::CBehaviorTreeMgr::ReleaseOldTree()
{
	for (auto it = m_mapOldBehaviorTree.begin(); it != m_mapOldBehaviorTree.end(); ++it)
	{
		m_oTreePool.ReleaseObj(it->second);
	}
	m_mapOldBehaviorTree.clear();
	
}

void BT::CBehaviorTreeMgr::ReleaseNewTree()
{
	for (auto it = m_mapNewBehaviorTree.begin(); it != m_mapNewBehaviorTree.end(); ++it)
	{
		m_oTreePool.ReleaseObj(it->second);
	}
	m_mapNewBehaviorTree.clear();
}

void BT::CBehaviorTreeMgr::RecycleOldTree()
{
	if (m_bUseNew) {
		ReleaseOldNode();
		ReleaseOldTree();
	}
	else {
		ReleaseNewNode();
		ReleaseNewTree();
	}

}

BT::ITreeNode* BT::CBehaviorTreeMgr::CreateNode(const char* pszName)
{
	ProductMaker<ITreeNode>* pNodeMaker = _node_factory.GetCreator(pszName);
	if (nullptr == pNodeMaker) {
		return nullptr;
	}
	ITreeNode* pNode = pNodeMaker->FetchObj();
	if (!pNode) {
		return nullptr;
	}
	pNode->SetNodeID(_GetNextNodeID());
	if (m_bUseNew) {
		m_mapNewTreeNode[pNode->GetNodeID()] = pNode;
	}
	else {
		m_mapOldTreeNode[pNode->GetNodeID()] = pNode;
	}
	
	return pNode;
}

void BT::CBehaviorTreeMgr::ReleaseOldNode()
{
	for (auto& pNode : m_mapOldTreeNode)
	{
		_ReleaseNodeObj(pNode.second);
	}
	m_mapOldTreeNode.clear();
}

void BT::CBehaviorTreeMgr::ReleaseNewNode()
{
	for (auto& pNode : m_mapNewTreeNode)
	{
		_ReleaseNodeObj(pNode.second);
	}
	m_mapNewTreeNode.clear();
}

CBehaviorTreeMgr * BT::CBehaviorTreeMgr::Back()
{
	m_stTree.pop();
	return this;
}

bool BT::CBehaviorTreeMgr::AddTreeNode(ITreeNode* pNode)
{
	if (pNode == NULL) {
		return false;
	}
	//先设置根节点
	if (!m_pTreeRoot) {
		m_pTreeRoot = pNode;
	}
	else
	{
		if (m_stTree.empty()) {
			return false;
		}
		ITreeNode* pParent = m_stTree.top();
		if (!pParent) {
			return false;
		}
		ENodeType eType = pParent->GetNodeType();
		//只有复合节点 和 循环节点 有子节点
		if (pParent->GetNodeType() == E_TYPE_COMPOSITE
			//|| pParent->GetNodeType() == E_TYPE_LOOP
			|| pParent->GetNodeType() == E_TYPE_SEQUENCE
			|| pParent->GetNodeType() == E_TYPE_SELECT) {
			pParent->AddChild(pNode);
		}
		else {
			return false;
		}
	}
	m_stTree.push(pNode);
	return true;
}

CBehaviorTree * BT::CBehaviorTreeMgr::FindBehaviorTree(uint32 nId)
{
	CBehaviorTree *pNode = NULL;
	if (m_bUseNew) {
		auto it = m_mapNewBehaviorTree.find(nId);
		if (it == m_mapNewBehaviorTree.end()) {
			return nullptr;
		}
		pNode = it->second;
	}
	else {
		auto it = m_mapOldBehaviorTree.find(nId);
		if (it == m_mapOldBehaviorTree.end()) {
			return nullptr;
		}
		pNode = it->second;
	}
	
	return pNode;
}

BT::ENodeStatus BT::CBehaviorTreeMgr::Test()
{
	if (!m_mapOldBehaviorTree.empty()) {
		auto it = m_mapOldBehaviorTree.begin();
		return it->second->Tick();
	}
	return E_NODE_FAILURE;
}
