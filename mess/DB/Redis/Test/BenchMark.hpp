
#include <chrono>
#include <random>
#include <future>
#include <algorithm>
#include "StopWatch.h"
#include "Log/Logger.h"
template <typename RedisInstance>
BenchmarkTest<RedisInstance>::BenchmarkTest(const BenchmarkOptions &opts,
                                            RedisInstance &instance) : _opts(opts), _redis(instance)
{
    REDIS_ASSERT(_opts.pool_size > 0 && _opts.thread_num > 0 && _opts.total_request_num > 0 && _opts.key_len > 0 && _opts.val_len > 0,
                 "Invalid benchmark test options.");

    _keys = _gen_keys();
    _value = _gen_value();
}

template <typename RedisInstance>
void BenchmarkTest<RedisInstance>::async_run(const RedisAsyncCbPtr &cb)
{
    _async_cleanup(cb);

    _run("ASYNC SET key value", [this, &cb](std::size_t idx, RedisInstance &redis) {
        redis->async_setString(cb, this->_key(idx), _value);
        //redis->setString(this->_key(idx), _value);
        
    });

    _run("ASYNC GET key", [this, &cb](std::size_t idx, RedisInstance &redis) {
        
        redis->async_getString(cb, this->_key(idx));
        
        
    });

    _async_cleanup(cb);

    _run("ASYNC INCR key", [this, &cb](std::size_t idx, RedisInstance &redis) {

        redis->async_incr(cb, this->_key(idx), 1);
        
    });

    _async_cleanup(cb);

    _run("ASYNC SADD key member", [this, &cb](std::size_t idx, RedisInstance &redis) {
        
        redis->async_addSetMember(cb, this->_key(idx), _value);
        
    });

    _async_cleanup(cb);
}
template <typename RedisInstance>
void BenchmarkTest<RedisInstance>::run()
{
    _cleanup();

    _run("SET key value", [this](std::size_t idx, RedisInstance &redis) {
        
        redis->setString(this->_key(idx), _value);
        
    });

    _run("GET key", [this](std::size_t idx, RedisInstance &redis) {
        std::string str;
        
        auto res = redis->getString(this->_key(idx), str);
        (void)res;
        
    });

    _cleanup();

    _run("INCR key", [this](std::size_t idx, RedisInstance &redis) {
        int64_t val;
        
        auto num = redis->incr(this->_key(idx), 1, val);
        (void)val, (void)num;
        
    });

    _cleanup();

    _run("SADD key member", [this](std::size_t idx, RedisInstance &redis) {
        bool bAdd;
        
        auto num = redis->addSetMember(this->_key(idx), _value, bAdd);
        (void)num;
        
    });

    _cleanup();
}

template <typename RedisInstance>
template <typename Func>
void BenchmarkTest<RedisInstance>::_run(const std::string &title, Func &&func)
{
    auto thread_num = _opts.thread_num;
    auto requests_per_thread = _opts.total_request_num / thread_num;
    auto total_request_num = requests_per_thread * thread_num;
    std::vector<std::future<std::pair<std::size_t, std::string>>> res;
    for (size_t i = 0; i < thread_num; ++i)
    {
        res.push_back(std::async(
            std::launch::async,
            [this](Func &&func, std::size_t request_num) {
                return this->_run(std::forward<Func>(func), request_num);
            },
            std::forward<Func>(func),
            requests_per_thread)
        );
    }

    auto total_in_msec = 0;
    auto start = std::chrono::steady_clock::now();
    for(auto& fut :res)
    {
        auto tmp = fut.get();
        auto avg = tmp.first * 1.0 / requests_per_thread;
        auto ops = static_cast<std::size_t>(1000 / avg);
        LOG_DEBUG("-----" << title << "-----");
        LOG_DEBUG(requests_per_thread << " requests cost " << tmp.first * 1.0 / 1000<< " seconds");
        LOG_DEBUG(ops << " requests per second");
        LOG_DEBUG("-----Performance-----");
        LOG_DEBUG(tmp.second);
    }
    auto stop = std::chrono::steady_clock::now();
    total_in_msec = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

    auto total_in_sec = total_in_msec * 1.0 / 1000;

    auto avg = total_in_msec * 1.0 / total_request_num;

    auto ops = static_cast<std::size_t>(1000 / avg);
    LOG_DEBUG("-----" << "total" << "-----");
    LOG_DEBUG(total_request_num << " requests cost " << total_in_sec << " seconds");
    LOG_DEBUG(ops << " requests per second\n");

}

template <typename RedisInstance>
template <typename Func>
std::pair<std::size_t, std::string> BenchmarkTest<RedisInstance>::_run(Func &&func, std::size_t request_num)
{
    auto redis = hiredis::CLIENT_MGR.getRedisClient();
    PerformanceWatch<> oWatch;
    auto start = std::chrono::steady_clock::now();
    for (auto idx = 0U; idx != request_num; ++idx)
    {
        func(idx, redis);
        oWatch.Check();
    }
    hiredis::CLIENT_MGR.releaseClient(redis);
    std::string perf = oWatch.CreatePerformanceInfo();
    auto stop = std::chrono::steady_clock::now();
    return std::make_pair(std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count(), perf);
}

template <typename RedisInstance>
std::vector<std::string> BenchmarkTest<RedisInstance>::_gen_keys() const
{
    const auto KEY_NUM = 100;
    std::vector<std::string> res;
    res.reserve(KEY_NUM);
    std::default_random_engine engine(std::random_device{}());
    std::uniform_int_distribution<int> uniform_dist(65, 122);
    for (auto i = 0; i != KEY_NUM; ++i)
    {
        std::string str;
        str.reserve(_opts.key_len);
        for (std::size_t j = 0; j != _opts.key_len; ++j)
        {
            str.push_back(static_cast<char>(uniform_dist(engine)));
        }
        res.push_back(str);
    }

    return res;
}

template <typename RedisInstance>
std::string BenchmarkTest<RedisInstance>::_gen_value() const
{
    return std::string(_opts.val_len, 'x');
}

template <typename RedisInstance>
void BenchmarkTest<RedisInstance>::_cleanup()
{
    for (const auto &key : _keys)
    {
        _redis->delString(key);
    }
}

template <typename RedisInstance>
void BenchmarkTest<RedisInstance>::_async_cleanup(const RedisAsyncCbPtr &cb)
{
    for (const auto &key : _keys)
    {
        _redis->async_delString(cb, key);
    }
}