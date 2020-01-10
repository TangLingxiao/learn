#ifndef __UTIL_H__
#define __UTIL_H__
#include <string>
namespace util
{
class NullType;
class EmptyType
{
};
template <class... Ts>
class MyTuple;

template <>
class MyTuple<>
{
};
template <class T0, class... Ts>
class MyTuple<T0, Ts...>
{
public:
    using data_type = T0;
    using next = MyTuple<Ts...>;
    MyTuple(T0 t0, Ts... t) : data_(t0), next_(t...) {}
    data_type data_;
    next next_;
};

template <int i, class T>
class MyTupleElem
{
public:
    using data_type = typename MyTupleElem<i - 1, typename T::next>::data_type;
    static data_type get(const T &t)
    {
        return MyTupleElem<i - 1, typename T::next>().get(t.next_);
    }
};

template <class T>
class MyTupleElem<0, T>
{
public:
    using data_type = typename T::data_type;
    static data_type get(const T &t)
    {
        return t.data_;
    }
};

template <int i, class T>
typename MyTupleElem<i, T>::data_type get(const T &t)
{
    return MyTupleElem<i, T>::get(t);
}

template <int i>
struct Int2Type
{
    enum
    {
        num = i
    };
};

template <typename T>
struct Type2Type
{
    using Type = T;
};

template <bool>
struct StaticAssert;
template <>
struct StaticAssert<true>
{
    StaticAssert(const std::string &sMsg) : m_sMsg(sMsg) {}
    std::string m_sMsg;
};

#define STATIC_ASSERT(expr, msg) \
    StaticAssert<bool(expr)> t(msg);

template <typename T, typename U>
struct Convertable
{
    struct Temp
    {
        char temp[2];
    };
    static T makeT();
    static Temp convert(U);
    static char convert(...);
    enum
    {
        can = sizeof(convert(makeT())) == sizeof(Temp),
        can2way = Convertable<U, T>::can,
        same = 0,
    };
};

template <typename T>
struct Convertable<T, T>
{
    enum
    {
        can = 1,
        can2way = 1,
        same = 1,
    };
};

#define SUPERSUBCLASS(T, U) \
    (Convertable<const U *, const T *>::can && !Convertable<const T *, const void *>::same && !Convertable<const T, const U>::same)
} // namespace util

#endif