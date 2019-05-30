#include "rankmgr.h"
#include <algorithm>
// 生成玩家唯一ID
// 4位服务器组ID+12位服务器区ID+32位当前时间(秒)+16位自增 (老的版本)
// 4位服务器组ID+12位服务器区ID+2位服务器子编号+32位当前时间(秒)+14位自增
//1-玩家id 2-道具id 3-邮件id 4-排行榜id

template<int>
uint64 GenerateUUID()
{
	static uint64 s_playerStartID = 0;
	/*uint64 curtime = time(NULL);
	uint64 result = 0;
	s_playerStartID = (s_playerStartID + 1) % 16384;
	uint64 mypipeid = CBasePipeMgr::Instance()->GetPipeId();
	SGNPipeId* pPipeID = (SGNPipeId*)&mypipeid;
	result += pPipeID->wRegion;
	result <<= 12;
	result += pPipeID->wGroup;
	result <<= 2;
	result += pPipeID->wIndex;
	result <<= 32;
	result += curtime;
	result <<= 14;
	result += s_playerStartID;
	return result;*/
	return s_playerStartID++;
}

 CRankList::CRankList(CRankList && oRh)noexcept
{
	m_oSkipList = std::move(oRh.m_oSkipList);
	m_mapList = std::move(oRh.m_mapList);
	m_eType = oRh.m_eType;
	m_nRankId = oRh.m_nRankId;
	m_nCapacity = oRh.m_nCapacity;
}

bool  CRankList::Init(ECommonRankType eType, int32 nCapacity, int32 nRankId /*= 0*/)
{
	m_nRankId = nRankId;
	m_eType = eType;
	m_nCapacity = std::min<int32>(nCapacity, MAX_RANK_NUM);
	auto* pSkipList = m_oSkipList.CreateList();
	return pSkipList != nullptr;
}

void  CRankList::InsertItem(uint64 nId, int32 nScore)
{
	auto it = m_mapList.find(nId);
	if (it == m_mapList.end())
	{
		int32 nListLen = m_oSkipList.GetLength();
		if (nListLen == m_nCapacity)
		{
			auto *pLast = m_oSkipList.GetElementByRank(m_nCapacity);
			if (pLast && pLast->nScore >= nScore)
			{
				return;
			}
			uint64 nDel = 0;
			m_oSkipList.PopBack(nDel);
			m_mapList.erase(nDel);
		}
		uint64 nCmp = GenerateUUID<4>();
		m_mapList[nId] = std::make_pair(nCmp, nScore);
		m_oSkipList.Insert(nId, nScore, nCmp);
	}
	else
	{
		if (nScore == it->second.second)
		{
			return;
		}
		int32 nOldScore = it->second.second;
		uint64 nOldCmp = it->second.first;
		m_oSkipList.Delete(nOldScore, nOldCmp);
		uint64 nCmp = GenerateUUID<4>();
		m_oSkipList.Insert(nId, nScore, nCmp);
		it->second = std::make_pair(nCmp, nScore);
	}

}

void  CRankList::GetRankItems(int32 nBegin, int32 nEnd, std::vector<SCommonRankItem>& vecItems)
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

int32  CRankList::GetRank(uint64 nId)
{
	auto it = m_mapList.find(nId);
	if (it == m_mapList.end())
	{
		return 0;
	}
	int32 nScore = it->second.second;
	uint64 nCmp = it->second.first;
	return m_oSkipList.GetRank(nScore, nCmp);
}
