#pragma once

#include "DB/Redis/RedisClient.h"
using namespace hiredis;
struct BenchmarkOptions {
    std::size_t pool_size = 5;
    std::size_t thread_num = 1;
    std::size_t total_request_num = 100000;
    std::size_t key_len = 20;
    std::size_t val_len = 1000 * 100;
};

template <typename RedisInstance>
class BenchmarkTest {
public:
    BenchmarkTest(const BenchmarkOptions &opts, RedisInstance &instance);

    ~BenchmarkTest() {
        _cleanup();
    }

    void run();

    void async_run(const RedisAsyncCbPtr &cb);
private:
    template <typename Func>
    void _run(const std::string &title, Func &&func);

    template <typename Func>
    std::pair<std::size_t, std::string> _run(Func &&func, std::size_t request_num);

    void _test_get();

    std::vector<std::string> _gen_keys() const;

    std::string _gen_value() const;

    void _cleanup();

    void _async_cleanup(const RedisAsyncCbPtr &cb);
    const std::string& _key(std::size_t idx) const {
        return _keys[idx % _keys.size()];
    }

    BenchmarkOptions _opts;

    RedisInstance& _redis;
    
    std::vector<std::string> _keys;
    std::string _value;
};

#include "BenchMark.hpp"