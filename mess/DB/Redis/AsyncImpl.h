public:
//string
int async_setString(const RedisAsyncCbPtr &cb, const std::string &key, const std::string &val);
int async_getString(const RedisAsyncCbPtr &cb, const std::string &key);
int async_getStringBatch(const RedisAsyncCbPtr &cb, const std::vector<std::string> &vKey);
int async_setStringEx(const RedisAsyncCbPtr &cb, const std::string &key, const std::string &val, uint expire);
int async_setStringWithOption(const RedisAsyncCbPtr &cb, const std::string &key, const std::string &val, SetCmdOption iOption, uint32_t iExpireMS);
int async_setExpire(const RedisAsyncCbPtr &cb, const std::string &sKey, int32_t iSeconds);
int async_setExpireAt(const RedisAsyncCbPtr &cb, const std::string &sKey, int64_t iTimestamp);
int async_setPersist(const RedisAsyncCbPtr &cb, const std::string &sKey);
int async_delString(const RedisAsyncCbPtr &cb, const std::string &key);
int async_delString(const RedisAsyncCbPtr &cb, const std::vector<std::string> &keys);

//int
int async_incr(const RedisAsyncCbPtr &cb, const std::string &key, int64_t iNum);
int async_decr(const RedisAsyncCbPtr &cb, const std::string &key, int64_t iNum);

// //set
int async_addSetMember(const RedisAsyncCbPtr &cb, const std::string &key, const std::string &val);
int async_delSetMember(const RedisAsyncCbPtr &cb, const std::string &key, const std::string &val);
int async_getSetMember(const RedisAsyncCbPtr &cb, const std::string &key);
int async_isMemberInset(const RedisAsyncCbPtr &cb, const std::string &key, const std::string &val);
int async_getSetCount(const RedisAsyncCbPtr &cb, const std::string &key);

// //list
int async_getListLen(const RedisAsyncCbPtr &cb, const std::string &sKey);
int async_pushListFront(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sValue);
int async_pushListFrontBatch(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::vector<std::string> &vValue);
int async_pushListBack(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sValue);
int async_pushListBackBatch(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::vector<std::string> &vValue);
int async_popListFront(const RedisAsyncCbPtr &cb, const std::string &sKey);
int async_popListBack(const RedisAsyncCbPtr &cb, const std::string &sKey);
int async_setListByIndex(const RedisAsyncCbPtr &cb, const std::string &sKey, int iIndex, const std::string &sValue);
int async_getListByRange(const RedisAsyncCbPtr &cb, const std::string &sKey, int iStartIndex, int iStopIndex);
int async_trimList(const RedisAsyncCbPtr &cb, const std::string &sKey, int iStartIndex, int iStopIndex);

// //hmap
int32_t async_getHashAll(const RedisAsyncCbPtr &cb, const std::string &sKey);
int32_t async_getHashKeys(const RedisAsyncCbPtr &cb, const std::string &sKey);
int32_t async_getHashField(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sField);
int32_t async_getHashFieldBatch(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::vector<std::string> &vField);
int32_t async_setHashField(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sField, const std::string &sValue);
int32_t async_setHashFieldBatch(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::map<std::string, std::string> &mFieldValue);
int32_t async_delHashField(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sField);
int32_t async_delHashFieldBatch(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::vector<std::string> &vField);

// //zset
int32_t async_addSortSet(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sMember, double fScore, SetCmdOption iOption = SetCmdOption_None);
int32_t async_addSortSetBatch(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::map<double, std::string> &mMemberScore, SetCmdOption iOption = SetCmdOption_None);
int32_t async_getSortSetCount(const RedisAsyncCbPtr &cb, const std::string &sKey);
int32_t async_getSortSetCountByRange(const RedisAsyncCbPtr &cb, const std::string &sKey, double fMin, double fMax, int64_t iRangeOption = RangeOption_BothClose);
int32_t async_incrSortSetMemberScore(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sMember, double fScore);
int32_t async_getSortSetByRange(const RedisAsyncCbPtr &cb, const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex);
int32_t async_getSortSetByRangeRev(const RedisAsyncCbPtr &cb, const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex);
int32_t async_getSortSetByScore(const RedisAsyncCbPtr &cb, const std::string &sKey, double fMin, double fMax, int64_t iRangeOption = RangeOption_BothClose, int64_t iOffset = 0, int64_t iCount = 0);
int32_t async_getSortSetByScoreRev(const RedisAsyncCbPtr &cb, const std::string &sKey, double fMin, double fMax, int64_t iRangeOption = RangeOption_BothClose, int64_t iOffset = 0, int64_t iCount = 0);
int32_t async_getSortSetMemberRank(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sMember);
int32_t async_getSortSetMemberRankRev(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sMember);
int32_t async_delSortSet(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sMember);
int32_t async_delSortSetBatch(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::vector<std::string> &vMember);
int32_t async_delSortSetByRank(const RedisAsyncCbPtr &cb, const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex);
int32_t async_delSortSetByScore(const RedisAsyncCbPtr &cb, const std::string &sKey, double fMin, double fMax, int64_t iRangeOption = RangeOption_BothClose);
int32_t async_getSortSetMemberScore(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sMember);
