#pragma once
#include <memory>
#include "reply.h"
namespace hiredis
{
    class RedisAsyncCallback : public std::enable_shared_from_this<RedisAsyncCallback>
    {
    public:
        ~RedisAsyncCallback() = default;
        virtual void onCall(int32_t ret, ReplyPtr &reply) = 0;
    };
    using RedisAsyncCbPtr = std::shared_ptr<RedisAsyncCallback>;
}