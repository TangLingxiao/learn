#include "rankmgr.h"

void IRankList::Init()
{
	m_oSkipList.CreateList();
}

void IRankList::InsertItem(uint64 nId, int32 nScore)
{
	auto it = m_mapList.find(nId);
	if (it == m_mapList.end())
	{
		int32 nListLen = m_oSkipList.GetLength();
		if (nListLen == MAX_RANK_NUM)
		{
			auto *pLast = m_oSkipList.GetElementByRank(MAX_RANK_NUM);
			if (pLast && pLast->nScore >= nScore)
			{
				return;
			}
			uint64 nDel = 0;
			m_oSkipList.PopBack(nDel);
			m_mapList.erase(nDel);
		}
		m_mapList[nId] = nScore;
		m_oSkipList.Insert(nId, nScore);
	}
	else
	{
		m_oSkipList.Delete(it->second, it->first);
		m_oSkipList.Insert(nId, nScore);
		it->second = nScore;
	}

}

void IRankList::GetRankItems(int32 nBegin, int32 nEnd, std::vector<SRankItem>& vecItems)
{
	std::vector<SSkipListNode* > vecElements;
	m_oSkipList.GetElementsByRank(nBegin, nEnd, vecElements);
	auto nRank = nBegin;
	for (auto* pElement : vecElements)
	{
		if (pElement)
		{
			vecItems.emplace_back(pElement->nKey, pElement->nScore, nRank);
			nRank++;
		}
	}
}

int32 IRankList::GetRank(uint64 nId)
{
	auto it = m_mapList.find(nId);
	if (it == m_mapList.end())
	{
		return 0;
	}
	
	return m_oSkipList.GetRank(it->second, it->first);
}
