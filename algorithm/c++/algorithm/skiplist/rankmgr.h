#ifndef _RANK_MGR_H_
#define _RANK_MGR_H_
#include "skiplist.h"
#include <map>

#define MAX_RANK_NUM (10000)

enum ECommonRankType {
	E_RANK_TYPE_NONE = 0,
	E_RANK_PLAYER_COMBAT = 1,
	E_RANK_HERO_COMBAT = 2,
	E_RANK_WAR_STAR = 3,
	E_RANK_TYPE_MAX = 4
};

struct SCommonRankItem
{
	SCommonRankItem(uint64 n1 = 0, int32 n2 = 0, int32 n3 = 0)
	{
		nId = n1;
		nScore = n2;
		nRank = n3;
	}
	uint64 nId = 0;
	int32 nScore = 0;
	int32 nRank = 0;
};

class CRankList
{
public:
	CRankList() = default;
	CRankList(CRankList& oRh) = delete;
	CRankList& operator=(CRankList&oRh) = delete;
	CRankList(CRankList&& oRh) noexcept;
	bool Init(ECommonRankType eType, int32 nCapacity, int32 nRankId = 0);
	void InsertItem(uint64 nId, int32 nScore);
	void GetRankItems(int32 nBegin, int32 nEnd, std::vector<SCommonRankItem>& vecItems);//1-based
	int32 GetRank(uint64 nId);//1-based
	ECommonRankType GetType() { return m_eType; }
	int32 GetRankId() { return m_nRankId; }
	int32 GetCapacity() { return m_nCapacity; }
private:
	CSkipList m_oSkipList;
	std::map<uint64, std::pair<uint64, int32> > m_mapList;
	ECommonRankType m_eType = E_RANK_TYPE_NONE;
	int32 m_nRankId = 0;
	int32 m_nCapacity = 0;
};
#endif // !_RANK_MGR_H_
