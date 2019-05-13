
#ifndef _TREE_MGR_H_
#define _TREE_MGR_H_
#include <stack>
#include <map>
#include "../xmlparser/xmlparser.h"
#include "../nodes/tree_node.h"
#include "../nodes/composite_node.h"
#include "../nodes/action_node_impl.h"
#include "../nodes/condition_node_impl.h"
#include "../nodes/select_target_impl.h"
#include "dynamicpool.h"
#include "singleton.h"
#include "commonheader.h"
namespace BT 
{
	template <typename A>
	class ProductMaker
	{
	public:
		typedef typename A::Base Base;

		virtual bool Init() = 0;
		virtual Base* FetchObj() = 0;

		virtual void ReleaseObj(Base*) = 0;

		virtual ~ProductMaker() { }
	};

	template <typename A>
	class PoolMaker : public ProductMaker<typename A::Base>
	{
	public:
		typedef typename A::Base Base;

		bool Init()
		{
			return _pool.Init(64, 16);
		}

		Base* FetchObj()
		{
			return _pool.FetchObj();
		}

		void ReleaseObj(Base* pBase)
		{
			_pool.ReleaseObj(static_cast<A*>(pBase));
		}
	private:
		CDynamicPoolEx<A> _pool;
	};

	template<typename I, typename C>
	class ProductFactory
	{
	public:
		void RegisterCteator(const I& id, C* creator)
		{
			creator->Init();
			_factory_maps[id] = creator;
		}
		C* GetCreator(const I& id)
		{
			auto it = _factory_maps.find(id);
			if (it != _factory_maps.end())
			{
				return it->second;
			}
			return NULL;
		}
	private:
		std::map<I, C*> _factory_maps;
	};
	//行为树构造器
	class CBehaviorTreeMgr
	{
		DECLARE_SINGLETON(CBehaviorTreeMgr);
	public:
		CBehaviorTreeMgr();
		~CBehaviorTreeMgr();
		bool Init();
		bool Load();
		bool Reload();
		ITreeNode* CreateNode(const char* pszName);
		void ReleaseOldNode();
		void ReleaseNewNode();
		//生成行为树，返回行为树根节点
		CBehaviorTree* CreateTree(int nTreeId);
		void ReleaseOldTree();
		void ReleaseNewTree();
		void RecycleOldTree();
		//退回上一个节点，把上一个节点弹出栈
		CBehaviorTreeMgr* Back();

		//添加子节点，通过栈操作
		bool AddTreeNode(ITreeNode* pNode);

		CBehaviorTree* FindBehaviorTree(uint32 nId);
		ENodeStatus Test();
	private:
		void _ReleaseNodeObj(ITreeNode* pNode);
		uint32 _GetNextTreeID();
		uint32 _GetNextNodeID();
	private:
		ITreeNode* m_pTreeRoot = nullptr;
		uint32 m_nTreeID = 0;
		uint32 m_nNodeID = 0;
		std::stack<ITreeNode*> m_stTree;
		CXMLParser m_oXmlParser;
		ProductFactory<std::string, ProductMaker<ITreeNode> > _node_factory;
		
		CDynamicPoolEx<CBehaviorTree>		m_oTreePool;
		
		std::map<uint32, ITreeNode*> m_mapOldTreeNode;
		std::map<uint32, ITreeNode*> m_mapNewTreeNode;
		std::map<uint32, CBehaviorTree*> m_mapOldBehaviorTree;
		std::map<uint32, CBehaviorTree*> m_mapNewBehaviorTree;
		bool m_bUseNew = false;
	};

}

#define _SNK_BEHAVIOR_TREE_() BT::CBehaviorTreeMgr::Instance()
#endif