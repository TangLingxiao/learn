#ifndef _RANK_MGR_H_
#define _RANK_MGR_H_
#include "skiplist.h"
#include <map>

#define MAX_RANK_NUM (10000)

struct SRankItem
{
	SRankItem(uint64 n1 = 0, int32 n2 = 0, int32 n3 = 0)
	{
		nId = n1;
		nScore = n2;
		nRank = n3;
	}
	uint64 nId = 0;
	int32 nScore = 0;
	int32 nRank = 0;
};

class IRankList
{
public:
	IRankList() = default;
	virtual ~IRankList(){}
	void Init();
	void InsertItem(uint64 nId, int32 nScore);
	void GetRankItems(int32 nBegin, int32 nEnd, std::vector<SRankItem>& vecItems);//1-based
	int32 GetRank(uint64 nId);//1-based
private:
	CSkipList m_oSkipList;
	std::map<uint64, int32> m_mapList;
};
#endif // !_RANK_MGR_H_
