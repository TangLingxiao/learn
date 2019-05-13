
#ifndef _TREE_NODE_H_
#define _TREE_NODE_H_
#include <set>
#include <vector>
#include <string>
#include <array>
#include "basic_types.h"
#include "type.h"
#include "creature.h"
#include "../../lib3rd/sgdp/src/tinyxml2/tinyxml2.h"
using namespace tinyxml2;
namespace BT
{
	class CBehaviorTree;
	//最基础的节点，其他所有节点都继承他
	class ITreeNode
	{
	public:
		typedef ITreeNode Base;

		ITreeNode() :m_nID(0),m_strName() {}
		virtual ~ITreeNode() {}
		virtual ENodeType GetNodeType()const = 0;
		virtual void AddChild(ITreeNode*) = 0;
		virtual ENodeStatus ExecuteTick(CBehaviorTree* pTree);
		virtual bool LoadXML(XMLElement* pNode) { return true; }
		ENodeStatus GetNodeStatus()const { return m_eStatus; }
		void SetNodeStatus(ENodeStatus eStatus) { m_eStatus = eStatus; }
		void SetNodeID(uint32 nID) { m_nID = nID; }
		const uint32 GetNodeID()const { return m_nID; }
		void SetNodeName(const std::string& strName) { m_strName = strName; }
		const char* GetNodeName()const { return m_strName.c_str(); }
		void SetStrNodeType(const std::string &strType) { m_strType = strType; }
		const char* GetStrNodeType()const { return m_strType.data(); }
	protected:
		virtual ENodeStatus tick(CBehaviorTree* pTree) = 0;
	private:
		uint32 m_nID;
		std::string m_strName;
		std::string m_strType;
		ENodeStatus m_eStatus = E_NODE_FAILURE;
	};

	//行为节点基类，其他具体行为节点继承他
	class CActionNode :public ITreeNode
	{
	public:
		CActionNode(){}
		virtual ~CActionNode() {}
		virtual ENodeType GetNodeType()const {
			return E_TYPE_ACTION;
		}

		virtual void AddChild(ITreeNode*)final {}
	};

	//条件节点基类，其他具体条件节点继承他
	class CConditionNode :public ITreeNode
	{
	public:
		CConditionNode(){}
		virtual ~CConditionNode() {}
		virtual ENodeType GetNodeType()const {
			return E_TYPE_CONDITION;
		}

		virtual void AddChild(ITreeNode*)final {}
	};

	//复合节点基类，select 和 sequence 节点继承他
	class CompositeNode :public ITreeNode
	{
	public:
		CompositeNode(){
			m_vecChildren.clear();
		}
		virtual ~CompositeNode() {}

		virtual ENodeType GetNodeType()const {
			return E_TYPE_COMPOSITE;
		}
		virtual void AddChild(ITreeNode* pChild) final;

		const std::vector<ITreeNode*>&GetChildren()const;

		const ITreeNode* GetChild(int index);

		const int GetChildrenNum()const;
	private:
		std::vector<ITreeNode*> m_vecChildren;
	};

	////特殊的节点，循环节点，只用作选择目标
	//class CLoopNode :public ITreeNode
	//{
	//public:
	//	CLoopNode():m_pChild(NULL), m_eCamp(E_CAMP_OTHER) {}
	//	~CLoopNode() = default;
	//	virtual ENodeType GetNodeType()const {
	//		return E_TYPE_LOOP;
	//	}
	//	void SetCamp(ECampCondition eCamp) {
	//		m_eCamp = eCamp;
	//	}
	//	virtual void AddChild(ITreeNode*)final;
	//protected:
	//	virtual ENodeStatus tick(CBehaviorTree* pTree);
	//private:
	//	ITreeNode* m_pChild;
	//	ECampCondition m_eCamp;
	//};

	//一棵行为树
	class CBehaviorTree
	{
	public:
		CBehaviorTree(){
			m_setTargetsObjId.clear();
		}
		~CBehaviorTree() {}
		ENodeStatus Tick();
		bool HaveRoot() { return m_pRoot ? true : false; }
		ITreeNode* SetTreeRoot(ITreeNode* pRoot);
		void SetTreeID(int nID) { m_nID = nID; }
		const int GetTreeID()const { return m_nID; }
		void SetTreeOwner(CCreature* pOwner) { m_pOwner = pOwner; }
		CCreature* GetTreeOwner()const { return m_pOwner; }
		//获取目标
		int GetTargetObjId();
		void ClearTargetObjId();

		int GetAssistPos()const { return m_nAssitPos; }
		void SetAssistPos(int nAssitPos) { m_nAssitPos = nAssitPos; }
		//设置选取的目标
		bool SetTargetObjId(std::set<int> &setTargets);
		int GetSkillType()const { return m_nSkillType; }
		void SetSkillType(int nSkillId) { m_nSkillType = nSkillId; }
		int GetExpSkillType()const { return m_nExpSkillType; }
		void SetExpSkillType(int nSkillId) { m_nExpSkillType = nSkillId; }
		void SetSelectAction(ESelectAction eAction) { m_eSelectAction = eAction; }
		ESelectAction GetSelectAction() { return m_eSelectAction; }
#if VERSION_CE
		void AddSelectAction(ESelectAction eAction);
		const std::array<ESelectAction, 2>& GetAllSelectAction() { return m_aSelectAction; }
#endif
		void ClearTree();
		const CfgSkill* GetSkillCfg()const { return m_pSkillCfg; }
		void SetSkillCfg(const CfgSkill* pCfg) { m_pSkillCfg = pCfg; }
	private:
		ITreeNode* m_pRoot = nullptr;
		int m_nID = 0;
		CCreature *m_pOwner = nullptr;
		std::set<int> m_setTargetsObjId;//选取目标的objid
		int m_nSkillType = 0;//选取的技能id
		const CfgSkill * m_pSkillCfg = nullptr;
		ESelectAction m_eSelectAction = E_SELECT_NONE;//选取的行为类型
		int m_nAssitPos = 0;//援护的目标
		int m_nExpSkillType = 0;//爆气技能id
#if VERSION_CE
		std::array<ESelectAction, 2> m_aSelectAction = { E_SELECT_NONE };
#endif
	};
}
#endif