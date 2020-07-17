#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "hiredis.h"

struct redisReply;
using Vector = std::vector<std::string>;
using Map = std::map<std::string,std::string>;
using VectorPair = std::vector<std::pair<std::string, double>>;
namespace hiredis
{
struct ReplyDeleter {
    void operator()(redisReply *reply) const {
        if (reply != nullptr) {
            freeReplyObject(reply);
        }
    }
};

using ReplyPtr = std::unique_ptr<redisReply, ReplyDeleter>;

inline bool is_error(redisReply &reply) {
    return reply.type == REDIS_REPLY_ERROR;
}

inline bool is_nil(redisReply &reply) {
    return reply.type == REDIS_REPLY_NIL;
}

inline bool is_string(redisReply &reply) {
    return reply.type == REDIS_REPLY_STRING;
}

inline bool is_status(redisReply &reply) {
    return reply.type == REDIS_REPLY_STATUS;
}

inline bool is_integer(redisReply &reply) {
    return reply.type == REDIS_REPLY_INTEGER;
}

inline bool is_array(redisReply &reply) {
    return reply.type == REDIS_REPLY_ARRAY;
}

template <typename T>
struct ParseTag{};

template <typename T>
inline T parser(ReplyPtr &reply)
{
    if(!reply)
    {
        throw std::runtime_error("redis connection closed");
    }
    if(is_error(*reply))
    {
        if(reply->str == nullptr)
        {
            throw std::runtime_error("null error reply");
        }
        throw std::runtime_error(std::string{reply->str, reply->len});
    }
    return parser(ParseTag<T>(), *reply);
}
std::string to_status(redisReply &reply);
void parser(ParseTag<void>, redisReply &reply);
std::string parser(ParseTag<std::string>, redisReply &reply);
bool parser(ParseTag<bool>, redisReply &reply);
double parser(ParseTag<double>, redisReply &reply);
long long parser(ParseTag<long long>, redisReply &reply);
Vector parser(ParseTag<Vector>, redisReply &reply);
Map parser(ParseTag<Map>, redisReply &reply);
VectorPair parser(ParseTag<VectorPair>, redisReply &reply);
}