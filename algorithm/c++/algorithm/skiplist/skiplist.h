#ifndef _SKIP_LIST_H_
#define _SKIP_LIST_H_
#include <iostream>
#include <vector>
//#include "type.h"
#define SKIPLIST_MAXLEVEL (32)
#define SKIPLIST_P (0.25)

using uint64 = unsigned long long;
using int32 = int;
struct SSkipListNode
{
	struct SSkipListLevel
	{
		SSkipListNode* pForward = nullptr;
		int32 nSpan = 0;//与下一个节点的跨度,计算排名
	};
	SSkipListLevel* pLevel = nullptr;
	SSkipListNode* pBackward = nullptr;

	uint64 nKey = 0;
	int32 nScore = 0;
	uint64 nCmp = 0;
};

class CSkipList
{
public:
	CSkipList() = default;
	CSkipList(CSkipList& oRh) = delete;
	CSkipList& operator=(CSkipList& oRh) = delete;
	CSkipList(CSkipList&& oRh) noexcept;
	CSkipList& operator=(CSkipList&& oRh) noexcept;
	~CSkipList()
	{
		_ReleaseList();
	}
	CSkipList* CreateList();
	SSkipListNode* Insert(uint64 nkey, int32 nScore, uint64 nCmp);
	bool Delete(int32 nScore, uint64 nCmp);
	int32 GetRank(int32 nScore, uint64 nCmp);
	SSkipListNode *GetElementByRank(int32 nRank);//1-based
	void PopBack(uint64 & nKey);
	void GetElementsByRank(int32 nBegin, int32 nEnd, std::vector<SSkipListNode*> &vecResult);
	int32 GetLength();
	int32 GetSize();
	int32 GetLevel();
	void DumpAll(std::vector<SSkipListNode*> & vecResult);
private:
	SSkipListNode* _CreateNode(int32 nLevel, uint64 nKey, int32 nScore, uint64 nCmp) noexcept;
	void _ReleaseNode(SSkipListNode* pNode)noexcept;

	void _ReleaseList();

	int _GetRandLevel();
	void _DeleteNode(SSkipListNode* pNode, SSkipListNode** update);
private:
	int32 m_nLevel = 1;
	int32 m_nLength = 0;
	SSkipListNode* m_pHeader = nullptr;//哨兵
	SSkipListNode* m_pTail = nullptr;
	int32 m_nSize = 0;
};


#endif
