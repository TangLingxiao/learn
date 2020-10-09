
#pragma once
#include <string>
#include <cstring>
#include <vector>
#include <exception>
#include <map>
#include <list>
// Refer to https://gcc.gnu.org/onlinedocs/cpp/_005f_005fhas_005finclude.html
#if __cplusplus >= 201703L
#if defined __has_include
#if __has_include(<string_view>)
#define REDIS_PLUS_PLUS_HAS_STRING_VIEW
#endif
#endif
#endif

#define REDIS_ASSERT(condition, msg) \
    redis_assert((condition), (msg), __FILE__, __LINE__)

inline void redis_assert(bool condition,
                         const std::string &msg,
                         const std::string &file,
                         int line)
{
    if (!condition)
    {
        auto err_msg = "ASSERT: " + msg + ". " + file + ":" + std::to_string(line);
        throw std::runtime_error(err_msg);
    }
}

#if defined REDIS_PLUS_PLUS_HAS_STRING_VIEW
#include <string_view>
using StringView = std::string_view;
#else
class StringView
{
public:
    constexpr StringView() noexcept = default;

    constexpr StringView(const char *data, std::size_t size) : _data(data), _size(size) {}

    StringView(const char *data) : _data(data), _size(std::strlen(data)) {}

    StringView(const std::string &str) : _data(str.data()), _size(str.size()) {}

    constexpr StringView(const StringView &) noexcept = default;

    StringView &operator=(const StringView &) noexcept = default;

    constexpr const char *data() const noexcept
    {
        return _data;
    }

    constexpr std::size_t size() const noexcept
    {
        return _size;
    }

private:
    const char *_data = nullptr;
    std::size_t _size = 0;
};
#endif

class CmdArg
{
public:
    inline CmdArg &operator<<(const StringView &s)
    {
        _temp_argv.emplace_back(s.data(), s.size());
        _argv.push_back(_temp_argv.back().data());
        _argv_len.push_back(s.size());
        return *this;
    }

    template <typename T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
    inline CmdArg &operator<<(T n)
    {
        _temp_argv.emplace_back(std::to_string(n));
        return operator<<(_temp_argv.back());
    }

    inline CmdArg &operator<<(const std::vector<std::string> &v)
    {
        for (const auto &s : v)
        {
            operator<<(s);
        }
        return *this;
    }

    inline CmdArg &operator<<(const std::map<std::string, std::string> &m)
    {
        for (const auto &s : m)
        {
            operator<<(s.first) << (s.second);
        }
        return *this;
    }

    template <typename T, typename std::enable_if<std::is_arithmetic<T>::value, int>::type = 0>
    inline CmdArg &operator<<(const std::map<T, std::string> &m)
    {
        for (const auto &s : m)
        {
            operator<<(s.first) << (s.second);
        }
        return *this;
    }

    template <typename Arg>
    inline CmdArg &append(const Arg &arg)
    {
        return operator<<(arg);
    }

    template <typename Arg, typename... Args>
    inline CmdArg &append(const Arg &arg, const Args &... argv)
    {
        operator<<(arg);
        return append(argv...);
    }

    size_t size() const
    {
        return _argv.size();
    }

    const char **argv()
    {
        return _argv.data();
    }

    const size_t *argv_len()
    {
        return _argv_len.data();
    }

private:
    std::vector<const char *> _argv;
    std::vector<size_t> _argv_len;
    std::list<std::string> _temp_argv;
};