#pragma once
#include <memory>
#include "Reply.h"
namespace hiredis
{
    enum RedisAsyncCallType
    {
        CALLTYPE_NONE = -1,
        CALLTYPE_SET,
        CALLTYPE_GET,
        CALLTYPE_DEL,
        CALLTYPE_GETBATCH,
        CALLTYPE_SETEX,
        CALLTYPE_SETWITHOPTION,
        CALLTYPE_SETEXPIRE,
        CALLTYPE_SETEXPIREAT,
        CALLTYPE_SETPERSIST,
        CALLTYPE_INCR,
        CALLTYPE_DECR,
        CALLTYPE_SADD,
        CALLTYPE_SREM,
        CALLTYPE_SMEMBERS,
        CALLTYPE_SISMEMBER,
        CALLTYPE_SCARD,
        CALLTYPE_LLEN,
        CALLTYPE_LPUSH,
        CALLTYPE_RPUSH,
        CALLTYPE_LPOP,
        CALLTYPE_RPOP,
        CALLTYPE_LSET,
        CALLTYPE_LRANGE,
        CALLTYPE_LTRIM,
        CALLTYPE_HGETALL,
        CALLTYPE_HKEYS,
        CALLTYPE_HGET,
        CALLTYPE_HMGET,
        CALLTYPE_HSET,
        CALLTYPE_HMSET,
        CALLTYPE_HDEL,
        CALLTYPE_ZADD,
        CALLTYPE_ZCARD,
        CALLTYPE_ZCOUNT,
        CALLTYPE_ZINCRBY,
        CALLTYPE_ZRANGE,
        CALLTYPE_ZREVRANGE,
        CALLTYPE_ZRANGEBYSCORE,
        CALLTYPE_ZREVRANGEBYSCORE,
        CALLTYPE_ZRANK,
        CALLTYPE_ZREVRANK,
        CALLTYPE_ZREM,
        CALLTYPE_ZREMRANGEBYRANK,
        CALLTYPE_ZREMRANGEBYSCORE,
        CALLTYPE_ZSCORE,
        CALLTYPE_MAX,
    };

    class RedisAsyncCallback : public std::enable_shared_from_this<RedisAsyncCallback>
    {
    public:
        RedisAsyncCallback() = default;
        virtual ~RedisAsyncCallback() = default;
        virtual void onCall(RedisAsyncCallType type, ReplyPtr &reply);
        virtual void callback_set(int32_t ret) { throw std::runtime_error("callback_set() overloading incorrect."); }
        virtual void callback_get(int32_t ret, const std::string &val) { throw std::runtime_error("callback_get() overloading incorrect."); }
        virtual void callback_del(int32_t ret) { throw std::runtime_error("callback_del() overloading incorrect."); }
        virtual void callback_getBatch(int32_t ret, const std::vector<std::string> &valus) { throw std::runtime_error("callback_getBatch() overloading incorrect."); }
        virtual void callback_setStringEx(int32_t ret) { throw std::runtime_error("callback_setStringEx() overloading incorrect."); }
        virtual void callback_setStringWithOption(int32_t ret, bool success) { throw std::runtime_error("callback_setStringWithOption() overloading incorrect."); }
        virtual void callback_setExpire(int32_t ret) { throw std::runtime_error("callback_setExpire() overloading incorrect."); }
        virtual void callback_setExpireAt(int32_t ret) { throw std::runtime_error("callback_setExpireAt() overloading incorrect."); }
        virtual void callback_setPersist(int32_t ret) { throw std::runtime_error("callback_setPersist() overloading incorrect."); }
        virtual void callback_incr(int32_t ret, int64_t val) { throw std::runtime_error("callback_incr() overloading incorrect."); }
        virtual void callback_decr(int32_t ret, int64_t val) { throw std::runtime_error("callback_decr() overloading incorrect."); }
        virtual void callback_addSetMember(int32_t ret, bool added) { throw std::runtime_error("callback_addSetMember() overloading incorrect."); }
        virtual void callback_delSetMember(int32_t ret, bool deleted) { throw std::runtime_error("callback_delSetMember() overloading incorrect."); }
        virtual void callback_getSetMember(int32_t ret, const std::vector<std::string> &values) { throw std::runtime_error("callback_getSetMember() overloading incorrect."); }
        virtual void callback_isMemberInset(int32_t ret, bool inset) { throw std::runtime_error("callback_isMemberInset() overloading incorrect."); }
        virtual void callback_getSetCount(int32_t ret, int64_t value) { throw std::runtime_error("callback_getSetCount() overloading incorrect."); }
        virtual void callback_getListLen(int32_t ret, int64_t len) { throw std::runtime_error("callback_getListLen() overloading incorrect."); }
        virtual void callback_lpush(int32_t ret, int64_t len) { throw std::runtime_error("callback_lpush() overloading incorrect."); }
        virtual void callback_rpush(int32_t ret, int64_t len) { throw std::runtime_error("callback_rpush() overloading incorrect."); }
        virtual void callback_lpop(int32_t ret, const std::string &value, bool has) { throw std::runtime_error("callback_lpop() overloading incorrect."); }
        virtual void callback_rpop(int32_t ret, const std::string &value, bool has) { throw std::runtime_error("callback_rpop() overloading incorrect."); }
        virtual void callback_setListByIndex(int32_t ret) { throw std::runtime_error("callback_setListByIndex() overloading incorrect."); }
        virtual void callback_getListByRange(int32_t ret, const std::vector<std::string> &) { throw std::runtime_error("callback_getListByRange() overloading incorrect."); }
        virtual void callback_trimList(int32_t ret) { throw std::runtime_error("callback_trimList() overloading incorrect."); };
        virtual void callback_getHashAll(int32_t ret, const std::map<std::string, std::string> &) { throw std::runtime_error("callback_getHashAll() overloading incorrect."); }
        virtual void callback_getHashKeys(int32_t ret, const std::vector<std::string> &) { throw std::runtime_error("callback_getHashKeys() overloading incorrect."); }
        virtual void callback_getHashField(int32_t ret, const std::string &, bool has) { throw std::runtime_error("callback_getHashField() overloading incorrect."); }
        virtual void callback_getHashFieldBatch(int32_t ret, const std::vector<std::string> &) { throw std::runtime_error("callback_getHashFieldBatch() overloading incorrect."); }
        virtual void callback_setHashField(int32_t ret) { throw std::runtime_error("callback_setHashField() overloading incorrect."); }
        virtual void callback_setHashFieldBatch(int32_t ret) { throw std::runtime_error("callback_setHashFieldBatch() overloading incorrect."); }
        virtual void callback_delHashField(int32_t ret) { throw std::runtime_error("callback_delHashField() overloading incorrect."); }
        virtual void callback_addSortSet(int32_t ret) { throw std::runtime_error("callback_addSortSet() overloading incorrect."); }
        virtual void callback_getSortSetCount(int32_t ret, int64_t count) { throw std::runtime_error("callback_getSortSetCount() overloading incorrect."); }
        virtual void callback_getSortSetCountByRange(int32_t ret, int64_t count) { throw std::runtime_error("callback_getSortSetCountByRange() overloading incorrect."); }
        virtual void callback_incrSortSetMemberScore(int32_t ret, double score) { throw std::runtime_error("callback_incrSortSetMemberScore() overloading incorrect."); }
        virtual void callback_getSortSetByRange(int32_t ret, const std::vector<std::pair<std::string, double>> &) { throw std::runtime_error("callback_getSortSetByRange() overloading incorrect."); }
        virtual void callback_getSortSetByRangeRev(int32_t ret, const std::vector<std::pair<std::string, double>> &) { throw std::runtime_error("callback_getSortSetByRangeRev() overloading incorrect."); }
        virtual void callback_getSortSetByScore(int32_t ret, const std::vector<std::pair<std::string, double>> &) { throw std::runtime_error("callback_getSortSetByScore() overloading incorrect."); }
        virtual void callback_getSortSetByScoreRev(int32_t ret, const std::vector<std::pair<std::string, double>> &) { throw std::runtime_error("callback_getSortSetByScoreRev() overloading incorrect."); }
        virtual void callback_getSortSetMemberRank(int32_t ret, int64_t rank, bool has) { throw std::runtime_error("callback_getSortSetMemberRank() overloading incorrect."); }
        virtual void callback_getSortSetMemberRankRev(int32_t ret, int64_t rank, bool has) { throw std::runtime_error("callback_getSortSetMemberRankRev() overloading incorrect."); }
        virtual void callback_delSortSet(int32_t ret) { throw std::runtime_error("callback_delSortSet() overloading incorrect."); }
        virtual void callback_delSortSetByRank(int32_t ret) { throw std::runtime_error("callback_delSortSetByRank() overloading incorrect."); }
        virtual void callback_delSortSetByScore(int32_t ret) { throw std::runtime_error("callback_delSortSetByScore() overloading incorrect."); }
        virtual void callback_getSortSetMemberScore(int32_t ret, double score, bool has) { throw std::runtime_error("callback_getSortSetMemberScore() overloading incorrect."); }

    };
    using RedisAsyncCbPtr = std::shared_ptr<RedisAsyncCallback>;

} // namespace hiredis