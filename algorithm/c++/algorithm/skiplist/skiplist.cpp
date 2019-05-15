#include "skiplist.h"

SSkipListNode * CSkipList::_CreateNode(int32 nLevel, uint64 nKey, int32 nScore)
{
	SSkipListNode* pNode = new SSkipListNode();
	if (nLevel > 0)
	{
		pNode->pLevel = new SSkipListNode::SSkipListLevel[nLevel];
	}
	pNode->nKey = nKey;
	pNode->nScore = nScore;
	m_nSize += sizeof(SSkipListNode) + nLevel * sizeof(SSkipListNode::SSkipListLevel);
	return pNode;
}

void CSkipList::_ReleaseNode(SSkipListNode * pNode)
{
	if (pNode)
	{
		if (pNode->pLevel)
		{
			delete[]pNode->pLevel;
		}
		pNode->pLevel = nullptr;
		delete pNode;
	}
	pNode = nullptr;
}

void CSkipList::_ReleaseList()
{
	auto *pNode = m_pHeader;
	while (pNode)
	{
		auto* pNext = pNode->pLevel[0].pForward;
		_ReleaseNode(pNode);
		pNode = pNext;
	}
}

int CSkipList::_GetRandLevel()
{
	int level = 1;
	while ((std::rand() & 0xFFFF) < (SKIPLIST_P * 0xFFFF))
		level += 1;
	return (level < SKIPLIST_MAXLEVEL) ? level : SKIPLIST_MAXLEVEL;
}

void CSkipList::_DeleteNode(SSkipListNode * pNode, SSkipListNode ** update)
{
	for (int i = 0; i < m_nLevel; ++i)
	{
		if (update[i]->pLevel[i].pForward == pNode)
		{
			update[i]->pLevel[i].nSpan += pNode->pLevel[i].nSpan - 1;
			update[i]->pLevel[i].pForward = pNode->pLevel[i].pForward;
		}
		else
		{
			update[i]->pLevel[i].nSpan -= 1;
		}
	}
	if (pNode->pLevel[0].pForward)
	{
		pNode->pLevel[0].pForward->pBackward = pNode->pBackward;
	}
	else
	{
		m_pTail = pNode->pBackward;
	}
	while (m_nLevel > 1 && m_pHeader->pLevel[m_nLevel - 1].pForward == nullptr)
	{
		m_nLevel--;
	}
	m_nLength--;
}

CSkipList* CSkipList::CreateList()
{
	m_nLevel = 1;
	m_pHeader = _CreateNode(SKIPLIST_MAXLEVEL, 0, 0);
	if (!m_pHeader)
	{
		return nullptr;
	}
	return this;
}

//不能插入同一个key,插入前必须先delete
//插入一个没有的key//按分值从大到小排序
SSkipListNode * CSkipList::Insert(uint64 nkey, int32 nScore)
{
	SSkipListNode *update[SKIPLIST_MAXLEVEL];//每层的前驱节点
	int32 rank[SKIPLIST_MAXLEVEL];//每层前驱节点的排名
	auto* pNode = m_pHeader;
	for (int i = m_nLevel - 1; i >= 0; --i)
	{
		rank[i] = i == m_nLevel - 1 ? 0 : rank[i + 1];
		while (pNode->pLevel[i].pForward && pNode->pLevel[i].pForward->nScore >= nScore)
		{
			rank[i] += pNode->pLevel[i].nSpan;
			pNode = pNode->pLevel[i].pForward;
		}
		update[i] = pNode;
	}

	int32 nLevel = _GetRandLevel();
	if (nLevel > m_nLevel)
	{
		for (int i = m_nLevel; i < nLevel; ++i)
		{
			rank[i] = 0;
			update[i] = m_pHeader;
			update[i]->pLevel[i].nSpan = m_nLength;
		}
		m_nLevel = nLevel;
	}
	pNode = _CreateNode(nLevel, nkey, nScore);
	for (int i = 0; i < nLevel; ++i)
	{
		pNode->pLevel[i].pForward = update[i]->pLevel[i].pForward;
		update[i]->pLevel[i].pForward = pNode;

		pNode->pLevel[i].nSpan = update[i]->pLevel[i].nSpan - (rank[0] - rank[i]);
		update[i]->pLevel[i].nSpan = (rank[0] - rank[i]) + 1;
	}

	for (int i = nLevel; i < m_nLevel; ++i)
	{
		update[i]->pLevel[i].nSpan++;
	}

	pNode->pBackward = (update[0] == m_pHeader) ? nullptr : update[0];
	if (pNode->pLevel[0].pForward)
	{
		pNode->pLevel[0].pForward->pBackward = pNode;
	}
	else
	{
		m_pTail = pNode;
	}
	m_nLength++;
	return pNode;
}

bool CSkipList::Delete(int32 nScore, uint64 nkey)
{
	SSkipListNode* update[SKIPLIST_MAXLEVEL];
	auto* pNode = m_pHeader;
	for (int i = m_nLevel - 1; i >= 0; --i)
	{
		while (pNode->pLevel[i].pForward && 
			(pNode->pLevel[i].pForward->nScore > nScore ||
				(pNode->pLevel[i].pForward->nScore == nScore &&
					pNode->pLevel[i].pForward->nKey != nkey)))
		{
			pNode = pNode->pLevel[i].pForward;
		}
		update[i] = pNode;
	}
	pNode = pNode->pLevel[0].pForward;
	if (pNode && pNode->nScore == nScore && pNode->nKey == nkey)//score可能会相同
	{
		_DeleteNode(pNode, update);
		_ReleaseNode(pNode);
		return true;
	}
	return false;
}

int32 CSkipList::GetRank(int32 nScore, uint64 nkey)
{
	int32 nTraversed = 0;
	auto* pNode = m_pHeader;
	for (int i = m_nLevel - 1; i >= 0; --i)
	{
		while (pNode->pLevel[i].pForward &&
			(pNode->pLevel[i].pForward->nScore > nScore ||
			(pNode->pLevel[i].pForward->nScore == nScore &&
				pNode->pLevel[i].pForward->nKey != nkey)))
		{
			nTraversed += pNode->pLevel[i].nSpan;
			pNode = pNode->pLevel[i].pForward;
		}
	}
	nTraversed++;
	pNode = pNode->pLevel[0].pForward;
	if (pNode && pNode->nScore == nScore && pNode->nKey == nkey)//score可能会相同
	{
		return nTraversed;
	}
	return 0;
}

SSkipListNode * CSkipList::GetElementByRank(int32 nRank)
{
	int32 nTraversed = 0;
	auto *pNode = m_pHeader;
	for (int i = m_nLevel - 1; i >= 0; --i)
	{
		while (pNode->pLevel[i].pForward && (nTraversed + pNode->pLevel[i].nSpan) <= nRank)
		{
			nTraversed += pNode->pLevel[i].nSpan;
			pNode = pNode->pLevel[i].pForward;
		}
		if (nTraversed == nRank)
		{
			return pNode;
		}
	}
	return nullptr;
}

void CSkipList::PopBack(uint64 & nKey)
{
	nKey = m_pTail->nKey;
	Delete(m_pTail->nScore, m_pTail->nKey);
}

void CSkipList::GetElementsByRank(int32 nBegin, int32 nEnd, std::vector<SSkipListNode*>& vecResult)
{
	int32 nTraversed = 0;
	auto *pNode = m_pHeader;
	for (int i = m_nLevel - 1; i >= 0; --i)
	{
		while (pNode->pLevel[i].pForward && (nTraversed + pNode->pLevel[i].nSpan) < nBegin)
		{
			nTraversed += pNode->pLevel[i].nSpan;
			pNode = pNode->pLevel[i].pForward;
		}
	}
	nTraversed++;
	pNode = pNode->pLevel[0].pForward;//第一个大于等于begin的
	while (pNode && nTraversed <= nEnd)
	{
		vecResult.emplace_back(pNode);
		nTraversed++;
		pNode = pNode->pLevel[0].pForward;
	}
}

int32 CSkipList::GetLength()
{
	return m_nLength;
}

int32 CSkipList::GetSize()
{
	return m_nSize;
}

int32 CSkipList::GetLevel()
{
	return m_nLevel;
}

