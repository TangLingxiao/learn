public:
//string
int setString(const std::string &key, const std::string &val);
int getString(const std::string &key, std::string &val);
int getStringBatch(const std::vector<std::string> &vKey, std::map<std::string, std::string> &mKeyValue);
int setStringEx(const std::string &key, const std::string &val, uint expire);
int setStringWithOption(const std::string &key, const std::string &val, SetCmdOption iOption, uint32_t iExpireMS, bool &bSuccess);

int32_t setExpire(const std::string &sKey, int32_t iSeconds);
int32_t setExpireAt(const std::string &sKey, int64_t iTimestamp);
int32_t setPersist(const std::string &sKey);

//int
int incr(const std::string &key, int64_t iNum, int64_t &iValue);
int decr(const std::string &key, int64_t iNum, int64_t &iValue);

//set
int addSetMember(const std::string &key, const std::string &val, bool &bAdded);
int delSetMember(const std::string &key, const std::string &val, bool &bDeleted);
int getSetMember(const std::string &key, std::vector<std::string> &vMembers);
int isMemberInset(const std::string &key, const std::string &val, bool &bInSet);
int getSetCount(const std::string &key, int64_t &intval);

//list
int getListLen(const std::string &sKey, int64_t &iLen);
int pushListFront(const std::string &sKey, const std::string &sValue, int64_t &iLen);
int pushListFrontBatch(const std::string &sKey, const std::vector<std::string> &vValue, int64_t &iLen);
int pushListBack(const std::string &sKey, const std::string &sValue, int64_t &iLen);
int pushListBackBatch(const std::string &sKey, const std::vector<std::string> &vValue, int64_t &iLen);
int popListFront(const std::string &sKey, std::string &sValue, bool &bHasValue);
int popListBack(const std::string &sKey, std::string &sValue, bool &bHasValue);
int setListByIndex(const std::string &sKey, int iIndex, const std::string &sValue);
int getListByRange(const std::string &sKey, int iStartIndex, int iStopIndex, std::vector<std::string> &vValue);
int trimList(const std::string &sKey, int iStartIndex, int iStopIndex);

//hmap
int32_t getHashAll(const std::string &sKey, std::map<std::string, std::string> &mFieldValue);
int32_t getHashKeys(const std::string &sKey, std::vector<std::string> &vField);
int32_t getHashField(const std::string &sKey, const std::string &sField, std::string &sValue, bool &bHasValue);
int32_t getHashFieldBatch(const std::string &sKey, const std::vector<std::string> &vField, std::map<std::string, std::string> &mFieldValue);
int32_t setHashField(const std::string &sKey, const std::string &sField, const std::string &sValue);
int32_t setHashFieldBatch(const std::string &sKey, const std::map<std::string, std::string> &mFieldValue);
int32_t delHashField(const std::string &sKey, const std::string &sField);
int32_t delHashFieldBatch(const std::string &sKey, const std::vector<std::string> &vField);

//zset
int32_t addSortSet(const std::string &sKey, const std::string &sMember, double fScore, SetCmdOption iOption = SetCmdOption_None);
int32_t addSortSetBatch(const std::string &sKey, const std::map<double, std::string> &mMemberScore, SetCmdOption iOption = SetCmdOption_None);
int32_t getSortSetCount(const std::string &sKey, int64_t &iCount);
int32_t getSortSetCountByRange(const std::string &sKey, double fMin, double fMax, int64_t &iCount, int64_t iRangeOption = RangeOption_BothClose);
int32_t incrSortSetMemberScore(const std::string &sKey, const std::string &sMember, double fScore, double &fNewScore);
int32_t getSortSetByRange(const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex, std::vector<std::pair<std::string, double>> &vMemberScore);
int32_t getSortSetByRangeRev(const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex, std::vector<std::pair<std::string, double>> &vMemberScore);
int32_t getSortSetByScore(const std::string &sKey, double fMin, double fMax, std::vector<std::pair<std::string, double>> &vMemberScore, int64_t iRangeOption = RangeOption_BothClose, int64_t iOffset = 0, int64_t iCount = 0);
int32_t getSortSetByScoreRev(const std::string &sKey, double fMin, double fMax, std::vector<std::pair<std::string, double>> &vMemberScore, int64_t iRangeOption = RangeOption_BothClose, int64_t iOffset = 0, int64_t iCount = 0);
int32_t getSortSetMemberRank(const std::string &sKey, const std::string &sMember, int64_t &iRank, bool &bHasMember);
int32_t getSortSetMemberRankRev(const std::string &sKey, const std::string &sMember, int64_t &iRank, bool &bHasMember);
int32_t delSortSet(const std::string &sKey, const std::string &sMember);
int32_t delSortSetBatch(const std::string &sKey, const std::vector<std::string> &vMember);
int32_t delSortSetByRank(const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex);
int32_t delSortSetByScore(const std::string &sKey, double fMin, double fMax, int64_t iRangeOption = RangeOption_BothClose);
int32_t getSortSetMemberScore(const std::string &sKey, const std::string &sMember, double &fScore, bool &bHasMember);
