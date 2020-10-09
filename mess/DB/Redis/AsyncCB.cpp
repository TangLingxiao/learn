#include "AsyncCB.h"
#include "Util/StringHelper.h"
#include "RedisClient.h"
#include "Log/Logger.h"
using namespace hiredis;

void RedisAsyncCallback::onCall(RedisAsyncCallType type, ReplyPtr &reply)
{
    try{
    switch (type)
    {
    case CALLTYPE_SET:
    {
        parser<void>(reply);
        callback_set(0);
    }
    break;
    case CALLTYPE_GET:
    {
        if (reply && is_nil(*reply))
        {
            callback_get(RedisResult_NoData, UtilString::getEmptyString());
        }
        else
        {
            auto val = parser<std::string>(reply);
            callback_get(0, val);
        }
    }
    break;
    case CALLTYPE_DEL:
    {
        parser<long long>(reply);
        callback_del(0);
    }
    break;
    case CALLTYPE_GETBATCH:
    {
        auto val = parser<std::vector<std::string>>(reply);
        callback_getBatch(0, val);
    }
    break;
    case CALLTYPE_SETEX:
    {
        parser<void>(reply);
        callback_setStringEx(0);
    }
    break;
    case CALLTYPE_SETWITHOPTION:
    {
        if (reply && is_nil(*reply))
        {
            callback_setStringWithOption(0, false);
        }
        else
        {
            parser<void>(reply);
            callback_setStringWithOption(0, true);
        }
    }
    break;
    case CALLTYPE_SETEXPIRE:
    {
        if (parser<long long>(reply) == 0) //redis原本返回值 = 0 代表失败
        {
            callback_setExpire(1);
        }
        else
        {
            callback_setExpire(0);
        }
    }
    break;
    case CALLTYPE_SETEXPIREAT:
    {
        if (parser<long long>(reply) == 0) //redis原本返回值 = 0 代表失败
        {
            callback_setExpireAt(1);
        }
        else
        {
            callback_setExpireAt(0);
        }
    }
    break;
    case CALLTYPE_SETPERSIST:
    {
        if (parser<long long>(reply) == 0) //redis原本返回值 = 0 代表失败
        {
            callback_setPersist(1);
        }
        else
        {
            callback_setPersist(0);
        }
    }
    break;
    case CALLTYPE_INCR:
    {
        auto iValue = parser<long long>(reply);
        callback_incr(0, iValue);
    }
    break;
    case CALLTYPE_DECR:
    {
        auto iValue = parser<long long>(reply);
        callback_decr(0, iValue);
    }
    break;
    case CALLTYPE_SADD:
    {
        auto intval = parser<long long>(reply);
        if (intval != 0)
        {
            callback_addSetMember(0, true);
        }
        else
        {
            callback_addSetMember(0, false);
        }
    }
    break;
    case CALLTYPE_SREM:
    {
        auto intval = parser<long long>(reply);
        if (intval != 0)
        {
            callback_delSetMember(0, true);
        }
        else
        {
            callback_delSetMember(0, false);
        }
    }
    break;
    case CALLTYPE_SMEMBERS:
    {
        auto vMembers = parser<std::vector<std::string>>(reply);
        callback_getSetMember(0, vMembers);
    }
    break;
    case CALLTYPE_SISMEMBER:
    {
        auto intval = parser<long long>(reply);
        if (intval == 1)
        { 
            callback_isMemberInset(0, true);
        }
        else
        {
            callback_isMemberInset(0, false);
        }
    }
    break;
    case CALLTYPE_SCARD:
    {
        auto intval = parser<long long>(reply);
        callback_getSetCount(0, intval);
    }
    break;
    case CALLTYPE_LLEN:
    {
        auto len = parser<long long>(reply);
        callback_getListLen(0, len);
    }
    break;
    case CALLTYPE_LPUSH:
    {
        auto len = parser<long long>(reply);
        callback_lpush(0, len);
    }
    break;
    case CALLTYPE_RPUSH:
    {
        auto len = parser<long long>(reply);
        callback_rpush(0, len);
    }
    break;
    case CALLTYPE_LPOP:
    {
        if (reply && is_nil(*reply))
        {
            callback_lpop(0, UtilString::getEmptyString(), false);
        }
        else
        {
            auto sValue = parser<std::string>(reply);
            callback_lpop(0, sValue, true);
        }
    }
    break;
    case CALLTYPE_RPOP:
    {
        if (reply && is_nil(*reply))
        {
            callback_rpop(0, UtilString::getEmptyString(), false);
        }
        else
        {
            auto sValue = parser<std::string>(reply);
            callback_rpop(0, sValue, true);
        }
    }
    break;
    case CALLTYPE_LSET:
    {
        parser<void>(reply);
        callback_setListByIndex(0);
    }
    break;
    case CALLTYPE_LRANGE:
    {
        auto vValue = parser<std::vector<std::string>>(reply);
        callback_getListByRange(0, vValue);
    }
    break;
    case CALLTYPE_LTRIM:
    {
        parser<void>(reply);
        callback_trimList(0);
    }
    break;
    case CALLTYPE_HGETALL:
    {
        auto mFieldValue = parser<std::map<std::string, std::string>>(reply);
        callback_getHashAll(0, mFieldValue);
    }
    break;
    case CALLTYPE_HKEYS:
    {
        auto vField = parser<std::vector<std::string>>(reply);
        callback_getHashKeys(0, vField);
    }
    break;
    case CALLTYPE_HGET:
    {
        if (reply && is_nil(*reply))
        {
            callback_getHashField(0, UtilString::getEmptyString(), false);
        }
        else
        {
            auto sValue = parser<std::string>(reply);
            callback_getHashField(0, sValue, true);
        }
    }
    break;
    case CALLTYPE_HMGET:
    {
        auto values = parser<std::vector<std::string>>(reply);
        callback_getHashFieldBatch(0, values);
    }
    break;
    case CALLTYPE_HSET:
    {
        parser<long long>(reply);
        callback_setHashField(0);
    }
    break;
    case CALLTYPE_HMSET:
    {
        parser<void>(reply);
        callback_setHashFieldBatch(0);
    }
    break;
    case CALLTYPE_HDEL:
    {
        parser<long long>(reply);
        callback_delHashField(0);
    }
    break;
    case CALLTYPE_ZADD:
    {
        parser<long long>(reply);
        callback_addSortSet(0);
    }
    break;
    case CALLTYPE_ZCARD:
    {
        auto count = parser<long long>(reply);
        callback_getSortSetCount(0, count);
    }
    break;
    case CALLTYPE_ZCOUNT:
    {
        auto iCount = parser<long long>(reply);
        callback_getSortSetCountByRange(0, iCount);
    }
    break;
    case CALLTYPE_ZINCRBY:
    {
        auto fNewScore = parser<double>(reply);
        callback_incrSortSetMemberScore(0, fNewScore);
    }
    break;
    case CALLTYPE_ZRANGE:
    {
        auto vMemberScore = parser<std::vector<std::pair<std::string, double>>>(reply);
        callback_getSortSetByRange(0, vMemberScore);
    }
    break;
    case CALLTYPE_ZREVRANGE:
    {
        auto vMemberScore = parser<std::vector<std::pair<std::string, double>>>(reply);
        callback_getSortSetByRangeRev(0, vMemberScore);
    }
    break;
    case CALLTYPE_ZRANGEBYSCORE:
    {
        auto vMemberScore = parser<std::vector<std::pair<std::string, double>>>(reply);
        callback_getSortSetByScore(0, vMemberScore);
    }
    break;
    case CALLTYPE_ZREVRANGEBYSCORE:
    {
        auto vMemberScore = parser<std::vector<std::pair<std::string, double>>>(reply);
        callback_getSortSetByScoreRev(0, vMemberScore);
    }
    break;
    case CALLTYPE_ZRANK:
    {
        if (reply && is_nil(*reply))
        {
            callback_getSortSetMemberRank(0, 0, false);
        }
        else
        {
            auto iRank = parser<long long>(reply);
            callback_getSortSetMemberRank(0, iRank, true);
        }
    }
    break;
    case CALLTYPE_ZREVRANK:
    {
        if (reply && is_nil(*reply))
        {
            callback_getSortSetMemberRankRev(0, 0, false);
        }
        else
        {
            auto iRank = parser<long long>(reply);
            callback_getSortSetMemberRankRev(0, iRank, true);
        }
    }
    break;
    case CALLTYPE_ZREM:
    {
        parser<long long>(reply);
        callback_delSortSet(0);
    }
    break;
    case CALLTYPE_ZREMRANGEBYRANK:
    {
        parser<long long>(reply);
        callback_delSortSetByRank(0);
    }
    break;
    case CALLTYPE_ZREMRANGEBYSCORE:
    {
        parser<long long>(reply);
        callback_delSortSetByScore(0);
    }
    break;
    case CALLTYPE_ZSCORE:
    {
        if (reply && is_nil(*reply))
        {
            callback_getSortSetMemberScore(0, 0, false);
        }
        else
        {
            auto fScore = parser<double>(reply);
            callback_getSortSetMemberScore(0, fScore, true);
        }
    }
    break;
    default:
        throw std::runtime_error("error calltype");
        break;
    }
    }catch(std::exception &e)
    {
        LOG_ERROR("exception:"<<e.what());
    }
}