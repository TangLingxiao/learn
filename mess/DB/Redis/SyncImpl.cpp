#include "RedisClient.h"
#include <cstring>
#include <cassert>
#include "Reply.h"

using namespace hiredis;
int Client::getString(const std::string &key, std::string &val)
{
    CmdArg argv;
    argv << "get" << key;
    auto reply = command(argv);
    if (reply && is_nil(*reply))
    {
        return RedisResult_NoData;
    }
    val = parser<std::string>(reply);
    return 0;
}

int Client::getStringBatch(const std::vector<std::string> &vKey, std::map<std::string, std::string> &mKeyValue)
{
    CmdArg argv;
    argv << "mget" << vKey;
    auto reply = command(argv);
    auto val = parser<std::vector<std::string>>(reply);
    if (val.size() != vKey.size())
    {
        throw std::runtime_error("getStringBatch parser error");
    }
    for (size_t i = 0; i < vKey.size(); ++i)
    {
        mKeyValue[vKey[i]] = val[i];
    }
    return 0;
}

int Client::setString(const std::string &key, const std::string &val)
{
    CmdArg argv;
    argv << "set" << key << val;
    auto reply = command(argv);
    parser<void>(reply);
    return 0;
}

int Client::setStringEx(const std::string &key, const std::string &val, uint expire)
{
    CmdArg argv;
    argv << "setex" << key << expire << val;
    auto reply = command(argv);
    parser<void>(reply);
    return 0;
}

int Client::setStringWithOption(const std::string &key, const std::string &val, SetCmdOption iOption, uint32_t iExpireMS, bool &bSuccess)
{
    CmdArg argv;
    argv << "set" << key << val;
    if (iExpireMS != 0)
    {
        argv << "PX" << iExpireMS;
    }
    if (iOption == SetCmdOption_NX)
    {
        argv << "NX";
    }
    else if (iOption == SetCmdOption_XX)
    {
        argv << "XX";
    }
    auto reply = command(argv);
    if (reply && is_nil(*reply))
    {
        bSuccess = false;
        return 0;
    }
    parser<void>(reply);
    bSuccess = true;
    return 0;
}

int32_t Client::setExpire(const std::string &sKey, int32_t iSeconds)
{
    CmdArg argv;
    argv << "expire" << sKey << iSeconds;
    auto reply = command(argv);
    if (parser<long long>(reply) == 0) //redis原本返回值 = 0 代表失败
    {
        return 1;
    }
    return 0;
}

int32_t Client::setExpireAt(const std::string &sKey, int64_t iTimestamp)
{
    CmdArg argv;
    argv << "expireat" << sKey << iTimestamp;
    auto reply = command(argv);
    if (parser<long long>(reply) == 0) //redis原本返回值 = 0 代表失败
    {
        return 1;
    }
    return 0;
}

int32_t Client::setPersist(const std::string &sKey)
{
    CmdArg argv;
    argv << "persist" << sKey;
    auto reply = command(argv);
    if (parser<long long>(reply) == 0) //redis原本返回值 = 0 代表失败
    {
        return 1;
    }
    return 0;
}

int Client::delString(const std::string &key)
{
    CmdArg argv;
    argv << "del" << key;
    auto reply = command(argv);
    parser<long long>(reply);
    return 0;
}

int Client::delString(const std::vector<std::string> &keys)
{
    CmdArg argv;
    argv << "del";
    for (const auto &key : keys)
    {
        argv << key;
    }
    auto reply = command(argv);
    parser<long long>(reply);
    return 0;
}

int Client::incr(const std::string &key, int64_t iNum, int64_t &iValue)
{
    CmdArg argv;
    argv << "incrby" << key << (iNum);
    auto reply = command(argv);
    iValue = parser<long long>(reply);
    return 0;
}

int Client::decr(const std::string &key, int64_t iNum, int64_t &iValue)
{
    CmdArg argv;
    argv << "decrby" << key << iNum;
    auto reply = command(argv);
    iValue = parser<long long>(reply);
    return 0;
}

int Client::addSetMember(const std::string &key, const std::string &val, bool &bAdded)
{
    int64_t intval{0};
    CmdArg argv;
    argv << "sadd" << key << val;
    auto reply = command(argv);
    intval = parser<long long>(reply);
    if (intval != 0)
    {
        bAdded = true;
    }
    return 0;
}

int Client::delSetMember(const std::string &key, const std::string &val, bool &bDeleted)
{
    int64_t intval{0};
    CmdArg argv;
    argv << "srem" << key << val;
    auto reply = command(argv);
    intval = parser<long long>(reply);
    if (intval != 0)
    {
        bDeleted = true;
    }
    return 0;
}

int Client::getSetMember(const std::string &key, std::vector<std::string> &vMembers)
{
    CmdArg argv;
    argv << "smembers" << key;
    auto reply = command(argv);
    vMembers = parser<std::vector<std::string>>(reply);
    return 0;
}

int Client::isMemberInset(const std::string &key, const std::string &val, bool &bInSet)
{
    bInSet = false;
    int64_t intval{0};
    CmdArg argv;
    argv << "sismember" << key << val;
    auto reply = command(argv);
    intval = parser<long long>(reply);
    if (intval == 1)
        bInSet = true;
    return 0;
}

int Client::getSetCount(const std::string &key, int64_t &intval)
{
    CmdArg argv;
    argv << "scard" << key;
    auto reply = command(argv);
    intval = parser<long long>(reply);
    return 0;
}

int Client::getListLen(const std::string &sKey, int64_t &iLen)
{
    CmdArg argv;
    argv << "llen" << sKey;
    auto reply = command(argv);
    iLen = parser<long long>(reply);
    return 0;
}

int Client::pushListFront(const std::string &sKey, const std::string &sValue, int64_t &iLen)
{
    CmdArg argv;
    argv << "lpush" << sKey << sValue;
    auto reply = command(argv);
    iLen = parser<long long>(reply);
    return 0;
}

int Client::pushListFrontBatch(const std::string &sKey, const std::vector<std::string> &vValue, int64_t &iLen)
{
    CmdArg argv;
    argv << "lpush" << sKey << vValue;
    auto reply = command(argv);
    iLen = parser<long long>(reply);
    return 0;
}

int Client::pushListBack(const std::string &sKey, const std::string &sValue, int64_t &iLen)
{
    CmdArg argv;
    argv << "rpush" << sKey << sValue;
    auto reply = command(argv);
    iLen = parser<long long>(reply);
    return 0;
}

int Client::pushListBackBatch(const std::string &sKey, const std::vector<std::string> &vValue, int64_t &iLen)
{
    CmdArg argv;
    argv << "rpush" << sKey << vValue;
    auto reply = command(argv);
    iLen = parser<long long>(reply);
    return 0;
}

int Client::popListFront(const std::string &sKey, std::string &sValue, bool &bHasValue)
{
    CmdArg argv;
    argv << "lpop" << sKey;
    auto reply = command(argv);
    if (reply && is_nil(*reply))
    {
        bHasValue = false;
        return 0;
    }
    sValue = parser<std::string>(reply);
    bHasValue = true;
    return 0;
}

int Client::popListBack(const std::string &sKey, std::string &sValue, bool &bHasValue)
{
    CmdArg argv;
    argv << "rpop" << sKey;
    auto reply = command(argv);
    if (reply && is_nil(*reply))
    {
        bHasValue = false;
        return 0;
    }
    sValue = parser<std::string>(reply);
    bHasValue = true;
    return 0;
}

int Client::setListByIndex(const std::string &sKey, int iIndex, const std::string &sValue)
{
    CmdArg argv;
    argv << "lset" << sKey << iIndex << sValue;
    auto reply = command(argv);
    parser<void>(reply);
    return 0;
}

int Client::getListByRange(const std::string &sKey, int iStartIndex, int iStopIndex, std::vector<std::string> &vValue)
{
    CmdArg argv;
    argv << "lrange" << sKey << iStartIndex << iStopIndex;
    auto reply = command(argv);
    vValue = parser<std::vector<std::string>>(reply);
    return 0;
}

int Client::trimList(const std::string &sKey, int iStartIndex, int iStopIndex)
{
    CmdArg argv;
    argv << "ltrim" << sKey << iStartIndex << iStopIndex;
    auto reply = command(argv);
    parser<void>(reply);
    return 0;
}

int32_t Client::getHashAll(const std::string &sKey, std::map<std::string, std::string> &mFieldValue)
{
    CmdArg argv;
    argv << "hgetall" << sKey;
    auto reply = command(argv);
    mFieldValue = parser<std::map<std::string, std::string>>(reply);
    return 0;
}

int32_t Client::getHashKeys(const std::string &sKey, std::vector<std::string> &vField)
{
    CmdArg argv;
    argv << "hkeys" << sKey;
    auto reply = command(argv);
    vField = parser<std::vector<std::string>>(reply);
    return 0;
}

int32_t Client::getHashField(const std::string &sKey, const std::string &sField, std::string &sValue, bool &bHasValue)
{
    CmdArg argv;
    argv << "hget" << sKey << sField;
    auto reply = command(argv);
    if (reply && is_nil(*reply))
    {
        bHasValue = false;
        return 0;
    }
    sValue = parser<std::string>(reply);
    bHasValue = true;
    return 0;
}

int32_t Client::getHashFieldBatch(const std::string &sKey, const std::vector<std::string> &vField, std::map<std::string, std::string> &mFieldValue)
{
    CmdArg argv;
    argv << "hmget" << sKey << vField;
    auto reply = command(argv);
    auto values = parser<std::vector<std::string>>(reply);
    if (vField.size() != values.size())
    {
        throw std::runtime_error("hmget size error");
    }
    for (size_t i = 0; i < vField.size(); ++i)
    {
        mFieldValue[vField[i]] = values[i];
    }
    return 0;
}

int32_t Client::setHashField(const std::string &sKey, const std::string &sField, const std::string &sValue)
{
    CmdArg argv;
    argv << "hset" << sKey << sField << sValue;
    auto reply = command(argv);
    parser<long long>(reply);
    return 0;
}

int32_t Client::setHashFieldBatch(const std::string &sKey, const std::map<std::string, std::string> &mFieldValue)
{
    CmdArg argv;
    argv << "hmset" << sKey << mFieldValue;
    auto reply = command(argv);
    parser<void>(reply);
    return 0;
}

int32_t Client::delHashField(const std::string &sKey, const std::string &sField)
{
    CmdArg argv;
    argv << "hdel" << sKey << sField;
    auto reply = command(argv);
    parser<long long>(reply);
    return 0;
}

int32_t Client::delHashFieldBatch(const std::string &sKey, const std::vector<std::string> &vField)
{
    CmdArg argv;
    argv << "hdel" << sKey << vField;
    auto reply = command(argv);
    parser<long long>(reply);
    return 0;
}

int32_t Client::addSortSet(const std::string &sKey, const std::string &sMember, double fScore, SetCmdOption iOption)
{
    CmdArg argv;
    argv << "zadd" << sKey;
    if (iOption == SetCmdOption_NX)
    {
        argv << "NX";
    }
    else if (iOption == SetCmdOption_XX)
    {
        argv << "XX";
    }
    argv << fScore << sMember;
    auto reply = command(argv);
    parser<long long>(reply);
    return 0;
}

int32_t Client::addSortSetBatch(const std::string &sKey, const std::map<double, std::string> &mMemberScore, SetCmdOption iOption)
{
    CmdArg argv;
    argv << "zadd" << sKey;
    if (iOption == SetCmdOption_NX)
    {
        argv << "NX";
    }
    else if (iOption == SetCmdOption_XX)
    {
        argv << "XX";
    }
    argv << mMemberScore;
    auto reply = command(argv);
    parser<long long>(reply);
    return 0;
}

int32_t Client::getSortSetCount(const std::string &sKey, int64_t &iCount)
{
    CmdArg argv;
    argv << "zcard" << sKey;
    auto reply = command(argv);
    iCount = parser<long long>(reply);
    return 0;
}

int32_t Client::getSortSetCountByRange(const std::string &sKey, double fMin, double fMax, int64_t &iCount, int64_t iRangeOption)
{
    CmdArg argv;
    argv << "zcount" << sKey;
    if (fMin > fMax)
    {
        std::swap(fMin, fMax);
    }

    std::string sArg1 = std::to_string(fMin);
    std::string sArg2 = std::to_string(fMax);
    if (iRangeOption & RangeOption_LeftOpen)
    {
        sArg1 = "(" + sArg1;
    }
    if (iRangeOption & RangeOption_RightOpen)
    {
        sArg2 = "(" + sArg2;
    }
    argv << sArg1 << sArg2;
    auto reply = command(argv);
    iCount = parser<long long>(reply);
    return 0;
}

int32_t Client::incrSortSetMemberScore(const std::string &sKey, const std::string &sMember, double fScore, double &fNewScore)
{
    CmdArg argv;
    argv << "zincrby" << sKey << fScore << sMember;
    auto reply = command(argv);
    fNewScore = parser<double>(reply);
    return 0;
}

int32_t Client::getSortSetByRange(const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex, std::vector<std::pair<std::string, double>> &vMemberScore)
{
    CmdArg argv;
    argv << "zrange" << sKey << iStartIndex << iStopIndex << "withscores";
    auto reply = command(argv);
    vMemberScore = parser<std::vector<std::pair<std::string, double>>>(reply);
    return 0;
}

int32_t Client::getSortSetByRangeRev(const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex, std::vector<std::pair<std::string, double>> &vMemberScore)
{
    CmdArg argv;
    argv << "zrevrange" << sKey << iStartIndex << iStopIndex << "withscores";
    auto reply = command(argv);
    vMemberScore = parser<std::vector<std::pair<std::string, double>>>(reply);
    return 0;
}

int32_t Client::getSortSetByScore(const std::string &sKey, double fMin, double fMax, std::vector<std::pair<std::string, double>> &vMemberScore, int64_t iRangeOption, int64_t iOffset, int64_t iCount)
{
    CmdArg argv;
    argv << "zrangebyscore" << sKey;
    if (fMin > fMax)
    {
        std::swap(fMin, fMax);
    }

    std::string sArg1 = std::to_string(fMin);
    std::string sArg2 = std::to_string(fMax);
    if (iRangeOption & RangeOption_LeftOpen)
    {
        sArg1 = "(" + sArg1;
    }
    if (iRangeOption & RangeOption_RightOpen)
    {
        sArg2 = "(" + sArg2;
    }
    argv << sArg1 << sArg2 << "withscores";
    if (iCount != 0)
    {
        argv << "limit" << iOffset << iCount;
    }
    auto reply = command(argv);
    vMemberScore = parser<std::vector<std::pair<std::string, double>>>(reply);
    return 0;
}

int32_t Client::getSortSetByScoreRev(const std::string &sKey, double fMin, double fMax, std::vector<std::pair<std::string, double>> &vMemberScore, int64_t iRangeOption, int64_t iOffset, int64_t iCount)
{
    CmdArg argv;
    argv << "zrevrangebyscore" << sKey;
    if (fMin > fMax)
    {
        std::swap(fMin, fMax);
    }
    std::string sArg1 = std::to_string(fMin);
    std::string sArg2 = std::to_string(fMax);
    if (iRangeOption & RangeOption_LeftOpen)
    {
        sArg1 = "(" + sArg1;
    }
    if (iRangeOption & RangeOption_RightOpen)
    {
        sArg2 = "(" + sArg2;
    }
    std::swap(sArg1, sArg2);
    argv << sArg1 << sArg2 << "withscores";
    if (iCount != 0)
    {
        argv << "limit" << iOffset << iCount;
    }
    auto reply = command(argv);
    vMemberScore = parser<std::vector<std::pair<std::string, double>>>(reply);
    return 0;
}

int32_t Client::getSortSetMemberRank(const std::string &sKey, const std::string &sMember, int64_t &iRank, bool &bHasMember)
{
    CmdArg argv;
    argv << "zrank" << sKey << sMember;
    auto reply = command(argv);
    if (reply && is_nil(*reply))
    {
        bHasMember = false;
        return 0;
    }
    iRank = parser<long long>(reply);
    bHasMember = true;
    return 0;
}

int32_t Client::getSortSetMemberRankRev(const std::string &sKey, const std::string &sMember, int64_t &iRank, bool &bHasMember)
{
    CmdArg argv;
    argv << "zrevrank" << sKey << sMember;
    auto reply = command(argv);
    if (reply && is_nil(*reply))
    {
        bHasMember = false;
        return 0;
    }
    iRank = parser<long long>(reply);
    bHasMember = true;
    return 0;
}

int32_t Client::delSortSet(const std::string &sKey, const std::string &sMember)
{
    CmdArg argv;
    argv << "zrem" << sKey << sMember;
    auto reply = command(argv);
    parser<long long>(reply);
    return 0;
}

int32_t Client::delSortSetBatch(const std::string &sKey, const std::vector<std::string> &vMember)
{
    CmdArg argv;
    argv << "zrem" << sKey << vMember;
    auto reply = command(argv);
    parser<long long>(reply);
    return 0;
}

int32_t Client::delSortSetByRank(const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex)
{
    CmdArg argv;
    argv << "zremrangebyrank" << sKey << iStartIndex << iStopIndex;
    auto reply = command(argv);
    parser<long long>(reply);
    return 0;
}

int32_t Client::delSortSetByScore(const std::string &sKey, double fMin, double fMax, int64_t iRangeOption)
{
    CmdArg argv;
    argv << "zremrangebyscore" << sKey;
    if (fMin > fMax)
    {
        std::swap(fMin, fMax);
    }

    std::string sArg1 = std::to_string(fMin);
    std::string sArg2 = std::to_string(fMax);
    if (iRangeOption & RangeOption_LeftOpen)
    {
        sArg1 = "(" + sArg1;
    }
    if (iRangeOption & RangeOption_RightOpen)
    {
        sArg2 = "(" + sArg2;
    }
    argv << sArg1 << sArg2;
    auto reply = command(argv);
    parser<long long>(reply);
    return 0;
}

int32_t Client::getSortSetMemberScore(const std::string &sKey, const std::string &sMember, double &fScore, bool &bHasMember)
{
    CmdArg argv;
    argv << "zscore" << sKey << sMember;
    auto reply = command(argv);
    if (reply && is_nil(*reply))
    {
        bHasMember = false;
        return 0;
    }
    fScore = parser<double>(reply);
    bHasMember = true;
    return 0;
}
