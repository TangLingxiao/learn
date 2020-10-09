#include "RedisClient.h"
#include <cstring>
#include <cassert>
#include "Reply.h"

using namespace hiredis;

int Client::async_setString(const RedisAsyncCbPtr &cb, const std::string &key, const std::string &val)
{
    CmdArg argv;
    argv << "set" << key << val;
    CLIENT_MGR.startAsync<CALLTYPE_SET>(cb, __FILE__, __LINE__, argv);
    return 0;
}
int Client::async_getString(const RedisAsyncCbPtr &cb, const std::string &key)
{
    CmdArg argv;
    argv << "get" << key;
    CLIENT_MGR.startAsync<CALLTYPE_GET>(cb, __FILE__, __LINE__, argv);
    return 0;
}
int Client::async_getStringBatch(const RedisAsyncCbPtr &cb, const std::vector<std::string> &vKey)
{
    CmdArg argv;
    argv << "mget" << vKey;
    CLIENT_MGR.startAsync<CALLTYPE_GETBATCH>(cb, __FILE__, __LINE__, argv);
    return 0;
}
int Client::async_setStringEx(const RedisAsyncCbPtr &cb, const std::string &key, const std::string &val, uint expire)
{
    CmdArg argv;
    argv << "setex" << key << expire << val;
    CLIENT_MGR.startAsync<CALLTYPE_SETEX>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_setStringWithOption(const RedisAsyncCbPtr &cb, const std::string &key, const std::string &val, SetCmdOption iOption, uint32_t iExpireMS)
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
    CLIENT_MGR.startAsync<CALLTYPE_SETWITHOPTION>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_setExpire(const RedisAsyncCbPtr &cb, const std::string &sKey, int32_t iSeconds)
{
    CmdArg argv;
    argv << "expire" << sKey << iSeconds;
    CLIENT_MGR.startAsync<CALLTYPE_SETEXPIRE>(cb, __FILE__, __LINE__, argv);
    return 0;
}
int Client::async_setExpireAt(const RedisAsyncCbPtr &cb, const std::string &sKey, int64_t iTimestamp)
{
    CmdArg argv;
    argv << "expireat" << sKey << iTimestamp;
    CLIENT_MGR.startAsync<CALLTYPE_SETEXPIREAT>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_setPersist(const RedisAsyncCbPtr &cb, const std::string &sKey)
{
    CmdArg argv;
    argv << "persist" << sKey;
    CLIENT_MGR.startAsync<CALLTYPE_SETPERSIST>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_delString(const RedisAsyncCbPtr &cb, const std::string &key)
{
    CmdArg argv;
    argv << "del" << key;
    CLIENT_MGR.startAsync<CALLTYPE_DEL>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_delString(const RedisAsyncCbPtr &cb, const std::vector<std::string> &keys)
{
    CmdArg argv;
    argv << "del" << keys;
    CLIENT_MGR.startAsync<CALLTYPE_DEL>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_incr(const RedisAsyncCbPtr &cb, const std::string &key, int64_t iNum)
{
    CmdArg argv;
    argv << "incrby" << key << (iNum);
    CLIENT_MGR.startAsync<CALLTYPE_INCR>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_decr(const RedisAsyncCbPtr &cb, const std::string &key, int64_t iNum)
{
    CmdArg argv;
    argv << "decrby" << key << iNum;
    CLIENT_MGR.startAsync<CALLTYPE_DECR>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_addSetMember(const RedisAsyncCbPtr &cb, const std::string &key, const std::string &val)
{
    CmdArg argv;
    argv << "sadd" << key << val;
    CLIENT_MGR.startAsync<CALLTYPE_SADD>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_delSetMember(const RedisAsyncCbPtr &cb, const std::string &key, const std::string &val)
{
    CmdArg argv;
    argv << "srem" << key << val;
    CLIENT_MGR.startAsync<CALLTYPE_SREM>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_getSetMember(const RedisAsyncCbPtr &cb, const std::string &key)
{
    CmdArg argv;
    argv << "smembers" << key;
    CLIENT_MGR.startAsync<CALLTYPE_SMEMBERS>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_isMemberInset(const RedisAsyncCbPtr &cb, const std::string &key, const std::string &val)
{
    CmdArg argv;
    argv << "sismember" << key << val;
    CLIENT_MGR.startAsync<CALLTYPE_SISMEMBER>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_getSetCount(const RedisAsyncCbPtr &cb, const std::string &key)
{
    CmdArg argv;
    argv << "scard" << key;
    CLIENT_MGR.startAsync<CALLTYPE_SCARD>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_getListLen(const RedisAsyncCbPtr &cb, const std::string &sKey)
{
    CmdArg argv;
    argv << "llen" << sKey;
    CLIENT_MGR.startAsync<CALLTYPE_LLEN>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_pushListFront(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sValue)
{
    CmdArg argv;
    argv << "lpush" << sKey << sValue;
    CLIENT_MGR.startAsync<CALLTYPE_LPUSH>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_pushListFrontBatch(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::vector<std::string> &vValue)
{
    CmdArg argv;
    argv << "lpush" << sKey << vValue;
    CLIENT_MGR.startAsync<CALLTYPE_LPUSH>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_pushListBack(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sValue)
{
    CmdArg argv;
    argv << "rpush" << sKey << sValue;
    CLIENT_MGR.startAsync<CALLTYPE_RPUSH>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_pushListBackBatch(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::vector<std::string> &vValue)
{
    CmdArg argv;
    argv << "rpush" << sKey << vValue;
    CLIENT_MGR.startAsync<CALLTYPE_RPUSH>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_popListFront(const RedisAsyncCbPtr &cb, const std::string &sKey)
{
    CmdArg argv;
    argv << "lpop" << sKey;
    CLIENT_MGR.startAsync<CALLTYPE_LPOP>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_popListBack(const RedisAsyncCbPtr &cb, const std::string &sKey)
{
    CmdArg argv;
    argv << "rpop" << sKey;
    CLIENT_MGR.startAsync<CALLTYPE_RPOP>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_setListByIndex(const RedisAsyncCbPtr &cb, const std::string &sKey, int iIndex, const std::string &sValue)
{
    CmdArg argv;
    argv << "lset" << sKey << iIndex << sValue;
    CLIENT_MGR.startAsync<CALLTYPE_LSET>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_getListByRange(const RedisAsyncCbPtr &cb, const std::string &sKey, int iStartIndex, int iStopIndex)
{
    CmdArg argv;
    argv << "lrange" << sKey << iStartIndex << iStopIndex;
    CLIENT_MGR.startAsync<CALLTYPE_LRANGE>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int Client::async_trimList(const RedisAsyncCbPtr &cb, const std::string &sKey, int iStartIndex, int iStopIndex)
{
    CmdArg argv;
    argv << "ltrim" << sKey << iStartIndex << iStopIndex;
    CLIENT_MGR.startAsync<CALLTYPE_LTRIM>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_getHashAll(const RedisAsyncCbPtr &cb, const std::string &sKey)
{
    CmdArg argv;
    argv << "hgetall" << sKey;
    CLIENT_MGR.startAsync<CALLTYPE_HGETALL>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_getHashKeys(const RedisAsyncCbPtr &cb, const std::string &sKey)
{
    CmdArg argv;
    argv << "hkeys" << sKey;
    CLIENT_MGR.startAsync<CALLTYPE_HKEYS>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_getHashField(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sField)
{
    CmdArg argv;
    argv << "hget" << sKey << sField;
    CLIENT_MGR.startAsync<CALLTYPE_HGET>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_getHashFieldBatch(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::vector<std::string> &vField)
{
    CmdArg argv;
    argv << "hmget" << sKey << vField;
    CLIENT_MGR.startAsync<CALLTYPE_HMGET>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_setHashField(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sField, const std::string &sValue)
{
    CmdArg argv;
    argv << "hset" << sKey << sField << sValue;
    CLIENT_MGR.startAsync<CALLTYPE_HSET>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_setHashFieldBatch(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::map<std::string, std::string> &mFieldValue)
{
    CmdArg argv;
    argv << "hmset" << sKey << mFieldValue;
    CLIENT_MGR.startAsync<CALLTYPE_HMSET>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_delHashField(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sField)
{
    CmdArg argv;
    argv << "hdel" << sKey << sField;
    CLIENT_MGR.startAsync<CALLTYPE_HDEL>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_delHashFieldBatch(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::vector<std::string> &vField)
{
    CmdArg argv;
    argv << "hdel" << sKey << vField;
    CLIENT_MGR.startAsync<CALLTYPE_HDEL>(cb, __FILE__, __LINE__, argv);
    return 0;
}
int32_t Client::async_addSortSet(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sMember, double fScore, SetCmdOption iOption)
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
    CLIENT_MGR.startAsync<CALLTYPE_ZADD>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_addSortSetBatch(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::map<double, std::string> &mMemberScore, SetCmdOption iOption)
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
    CLIENT_MGR.startAsync<CALLTYPE_ZADD>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_getSortSetCount(const RedisAsyncCbPtr &cb, const std::string &sKey)
{
    CmdArg argv;
    argv << "zcard" << sKey;
    CLIENT_MGR.startAsync<CALLTYPE_ZCARD>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_getSortSetCountByRange(const RedisAsyncCbPtr &cb, const std::string &sKey, double fMin, double fMax, int64_t iRangeOption)
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
    CLIENT_MGR.startAsync<CALLTYPE_ZCOUNT>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_incrSortSetMemberScore(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sMember, double fScore)
{
    CmdArg argv;
    argv << "zincrby" << sKey << fScore << sMember;
    CLIENT_MGR.startAsync<CALLTYPE_ZINCRBY>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_getSortSetByRange(const RedisAsyncCbPtr &cb, const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex)
{
    CmdArg argv;
    argv << "zrange" << sKey << iStartIndex << iStopIndex << "withscores";

    CLIENT_MGR.startAsync<CALLTYPE_ZRANGE>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_getSortSetByRangeRev(const RedisAsyncCbPtr &cb, const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex)
{
    CmdArg argv;
    argv << "zrevrange" << sKey << iStartIndex << iStopIndex << "withscores";

    CLIENT_MGR.startAsync<CALLTYPE_ZREVRANGE>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_getSortSetByScore(const RedisAsyncCbPtr &cb, const std::string &sKey, double fMin, double fMax, int64_t iRangeOption, int64_t iOffset, int64_t iCount)
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
    CLIENT_MGR.startAsync<CALLTYPE_ZRANGEBYSCORE>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_getSortSetByScoreRev(const RedisAsyncCbPtr &cb, const std::string &sKey, double fMin, double fMax, int64_t iRangeOption, int64_t iOffset, int64_t iCount)
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
    CLIENT_MGR.startAsync<CALLTYPE_ZREVRANGEBYSCORE>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_getSortSetMemberRank(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sMember)
{
    CmdArg argv;
    argv << "zrank" << sKey << sMember;
    CLIENT_MGR.startAsync<CALLTYPE_ZRANK>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_getSortSetMemberRankRev(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sMember)
{
    CmdArg argv;
    argv << "zrevrank" << sKey << sMember;
    CLIENT_MGR.startAsync<CALLTYPE_ZREVRANK>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_delSortSet(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sMember)
{
    CmdArg argv;
    argv << "zrem" << sKey << sMember;
    CLIENT_MGR.startAsync<CALLTYPE_ZREM>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_delSortSetBatch(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::vector<std::string> &vMember)
{
    CmdArg argv;
    argv << "zrem" << sKey << vMember;
    CLIENT_MGR.startAsync<CALLTYPE_ZREM>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_delSortSetByRank(const RedisAsyncCbPtr &cb, const std::string &sKey, int32_t iStartIndex, int32_t iStopIndex)
{
    CmdArg argv;
    argv << "zremrangebyrank" << sKey << iStartIndex << iStopIndex;

    CLIENT_MGR.startAsync<CALLTYPE_ZREMRANGEBYRANK>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_delSortSetByScore(const RedisAsyncCbPtr &cb, const std::string &sKey, double fMin, double fMax, int64_t iRangeOption)
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
    CLIENT_MGR.startAsync<CALLTYPE_ZREMRANGEBYSCORE>(cb, __FILE__, __LINE__, argv);
    return 0;
}

int32_t Client::async_getSortSetMemberScore(const RedisAsyncCbPtr &cb, const std::string &sKey, const std::string &sMember)
{
    CmdArg argv;
    argv << "zscore" << sKey << sMember;
    CLIENT_MGR.startAsync<CALLTYPE_ZSCORE>(cb, __FILE__, __LINE__, argv);
    return 0;
}
