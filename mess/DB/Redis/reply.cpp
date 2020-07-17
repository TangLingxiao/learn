#include "reply.h"
#include "client.h"
namespace hiredis
{
std::string to_status(redisReply &reply) {
    if(is_error(reply))
    {
        if(reply.str == nullptr)
        {
            throw std::runtime_error("null error reply");
        }
        throw std::runtime_error(std::string{reply.str, reply.len});
    }
    if (!is_status(reply)) {
        throw std::runtime_error("redisReply Type Error");
    }

    if (reply.str == nullptr) {
        throw std::runtime_error("a null status reply");
    }
    return std::string(reply.str, reply.len);
}

void parser(ParseTag<void>, redisReply &reply)
{
    if(!is_status(reply))
    {
        throw std::runtime_error("redisReply Type Error");
    }
    if(reply.str == nullptr)
    {
        throw std::runtime_error("a null status reply");
    }
    static std::string ok = "OK";
    if(reply.len == 0 || strcasecmp(reply.str, "OK") != 0)
    {
        throw std::runtime_error("status error:" + std::string{reply.str, reply.len});
    }
}

std::string parser(ParseTag<std::string>, redisReply &reply)
{
    if(!is_string(reply) && !is_nil(reply))
    {
        throw std::runtime_error("redisReply Type Error");
    }
    
    if(is_nil(reply))
    {
        return std::string{};
    }
    
    if(reply.str == nullptr)
    {
        throw std::runtime_error("a null string reply");
    }
    return std::string{reply.str, reply.len};
}

bool parser(ParseTag<bool>, redisReply &reply)
{
    return parser(ParseTag<long long>(), reply) == 1?true:false;
}

double parser(ParseTag<double>, redisReply &reply)
{
    return std::stod(parser(ParseTag<std::string>(), reply));
}

long long parser(ParseTag<long long>, redisReply &reply)
{
    if(!is_integer(reply))
    {
        throw std::runtime_error("redisReply Type Error");
    }
    return reply.integer;
}

Vector parser(ParseTag<Vector>, redisReply &reply)
{
    if(!is_array(reply))
    {
        throw std::runtime_error("redisReply Type Error");
    }
    Vector values;
    for (size_t i = 0; i < reply.elements; ++i)
    {
        values.emplace_back(std::string{reply.element[i]->str, reply.element[i]->len});
    }
    return values;
}

Map parser(ParseTag<Map>, redisReply &reply)
{
    if(!is_array(reply))
    {
        throw std::runtime_error("redisReply Type Error");
    }
    Map values;
    std::string key;
    for (size_t i = 0; i < reply.elements; ++i)
    {
        auto item = reply.element[i];
        if(!is_string(*item))
        {
            throw std::runtime_error("redisReply Type Error");
        }
        if(i % 2 == 0)
        {
            key.assign(item->str, item->len);
        }
        else
        {
            values[key] = std::string{item->str, item->len};
        }
    }
    return values;
}

VectorPair parser(ParseTag<VectorPair>, redisReply &reply)
{
    if(!is_array(reply))
    {
        throw std::runtime_error("redisReply Type Error");
    }
    VectorPair values;
    std::pair<std::string, double> val;
    for (size_t i = 0; i < reply.elements; ++i)
    {
        auto item = reply.element[i];
        if(!is_string(*item))
        {
            throw std::runtime_error("redisReply Type Error");
        }
        if(i % 2 == 0)
        {
            val.first.assign(item->str, item->len);
        }
        else
        {
            val.second = std::stod(std::string{item->str, item->len});
            values.emplace_back(std::move(val));
        }
    }
    return values;
}

}