public:
//string
int async_setString(const RedisAsyncCbPtr &cb, const std::string &key, const std::string &val);
int async_getString(const RedisAsyncCbPtr &cb, const std::string &key);

// int async_getStringBatch(const std::vector<std::string> &vKey, std::map<std::string, std::string> &mKeyValue);
// int async_setStringEx(const std::string &key, const std::string &val, uint expire);
// int async_setStringWithOption(const std::string &key, const std::string &val, SetCmdOption iOption, uint32_t iExpireMS, bool &bSuccess);

// //int
// int async_incr(const std::string &key, int64_t iNum, int64_t &iValue);
// int async_decr(const std::string &key, int64_t iNum, int64_t &iValue);

// //set
// int async_addSetMember(const std::string &key, const std::string &val, bool &bAdded);
// int async_delSetMember(const std::string &key, const std::string &val, bool &bDeleted);
// int async_getSetMember(const std::string &key, std::vector<std::string> &vMembers);
// int async_isMemberInset(const std::string &key, const std::string &val, bool &bInSet);
// int async_getSetCount(const std::string &key, int64_t &intval);

// //list
// int async_getListLen(const std::string &sKey, int64_t &iLen);
// int async_pushListFront(const std::string &sKey, const std::string &sValue, int64_t &iLen);
// int async_pushListFrontBatch(const std::string &sKey, const std::vector<std::string> &vValue, int64_t &iLen);
// int async_pushListBack(const std::string &sKey, const std::string &sValue, int64_t &iLen);
// int async_pushListBackBatch(const std::string &sKey, const std::vector<std::string> &vValue, int64_t &iLen);
// int async_popListFront(const std::string &sKey, std::string &sValue, bool &bHasValue);
// int async_popListBack(const std::string &sKey, std::string &sValue, bool &bHasValue);
// int async_setListByIndex(const std::string &sKey, int iIndex, const std::string &sValue);
// int async_getListByRange(const std::string &sKey, int iStartIndex, int iStopIndex, std::vector<std::string> &vValue);
// int async_trimList(const std::string &sKey, int iStartIndex, int iStopIndex);

// //hmap
// int32_t async_getHashAll(const std::string &sKey, std::map<std::string, std::string> &mFieldValue);
// int32_t async_getHashKeys(const std::string &sKey, std::vector<std::string> &vField);
// int32_t async_getHashField(const std::string &sKey, const std::string &sField, std::string &sValue, bool &bHasValue);
// int32_t async_getHashFieldBatch(const std::string &sKey, const std::vector<std::string> &vField, std::map<std::string, std::string> &mFieldValue);
// int32_t async_setHashField(const std::string &sKey, const std::string &sField, const std::string &sValue);
// int32_t async_setHashFieldBatch(const std::string &sKey, const std::map<std::string, std::string> &mFieldValue);
// int32_t async_delHashField(const std::string &sKey, const std::string &sField);
// int32_t async_delHashFieldBatch(const std::string &sKey, const std::vector<std::string> &vField);

// //zset
// int32_t async_addSortSet(const std::string &sKey, const std::string &sMember, double fScore, SetCmdOption iOption = SetCmdOption_None);
// int32_t async_addSortSetBatch(const std::string &sKey, const std::map<double, std::string> &mMemberScore, SetCmdOption iOption = SetCmdOption_None);
// int32_t async_getSortSetCount(const std::string &sKey, int64_t &iCount);
// int32_t async_getSortSetCountByRange(const std::string &sKey, double fMin, double fMax, int64_t &iCount, int64_t iRangeOption = RangeOption_BothClose);
// int32_t async_incrSortSetMemberScore(const std::string &sKey, const std::string &sMember, double fScore, double &fNewScore);
// int32_t async_getSortSetByRange(const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex, std::vector<std::pair<std::string, double>> &vMemberScore);
// int32_t async_getSortSetByRangeRev(const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex, std::vector<std::pair<std::string, double>> &vMemberScore);
// int32_t async_getSortSetByScore(const std::string &sKey, double fMin, double fMax, std::vector<std::pair<std::string, double>> &vMemberScore, int64_t iRangeOption = RangeOption_BothClose, int64_t iOffset = 0, int64_t iCount = 0);
// int32_t async_getSortSetByScoreRev(const std::string &sKey, double fMin, double fMax, std::vector<std::pair<std::string, double>> &vMemberScore, int64_t iRangeOption = RangeOption_BothClose, int64_t iOffset = 0, int64_t iCount = 0);
// int32_t async_getSortSetMemberRank(const std::string &sKey, const std::string &sMember, int64_t &iRank, bool &bHasMember);
// int32_t async_getSortSetMemberRankRev(const std::string &sKey, const std::string &sMember, int64_t &iRank, bool &bHasMember);
// int32_t async_delSortSet(const std::string &sKey, const std::string &sMember);
// int32_t async_delSortSetBatch(const std::string &sKey, const std::vector<std::string> &vMember);
// int32_t async_delSortSetByRank(const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex);
// int32_t async_delSortSetByScore(const std::string &sKey, double fMin, double fMax, int64_t iRangeOption = RangeOption_BothClose);
// int32_t async_getSortSetMemberScore(const std::string &sKey, const std::string &sMember, double &fScore, bool &bHasMember);
