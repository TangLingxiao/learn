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

//U derived from T
#define SUPERSUBCLASS(T, U) \
    (Convertable<const U *, const T *>::can && !Convertable<const T *, const void *>::same && !Convertable<const T, const U>::same)

template <bool, typename T, typename U>
struct Select
{
    enum
    {
        value = 0
    };
    using Result = U;
};

template<typename T, typename U>
struct Select<true, T, U>
{
    enum
    {
        value = 1
    };
    using Result = T;
};

template <typename T, typename U>
class TypeList
{
    using HEAD = T;
    using TAIL = U;
};

template <typename typelist>
class Length;

template <typename T, typename U>
class Length<TypeList<T, U>>
{
public:
    enum
    {
        value = 1 + Length<U>::value,
    };
};
template <>
class Length<NullType>
{
public:
    enum
    {
        value = 0,
    };
};

template <typename typelist, int index>
struct TypeAt;
template <typename T, typename U, int index>
struct TypeAt<TypeList<T, U>, index>
{
    using Type = typename TypeAt<U, index - 1>::Type;
};
template <typename T, typename U>
struct TypeAt<TypeList<T, U>, 0>
{
    using Type = T;
};

template <typename typelist, typename T>
struct IndexOf;

template <typename HEAD, typename TAIL, typename T>
struct IndexOf<TypeList<HEAD, TAIL>, T>
{
    enum
    {
        temp = IndexOf<TAIL, T>::value,
        value = temp == -1 ? -1 : 1 + temp,
    };
};
template <typename T, typename U>
struct IndexOf<TypeList<T, U>, T>
{
    enum
    {
        value = 0
    };
};

template <typename typelist, typename T>
struct Append;
template <>
struct Append<NullType, NullType>
{
    using Result = NullType;
};
template <typename T>
struct Append<NullType, T>
{
    using Result = TypeList<T, NullType>;
};

template <typename T, typename U>
struct Append<NullType, TypeList<T, U>>
{
    using Result = TypeList<T, U>;
};
template <typename HEAD, typename TAIL, typename T>
struct Append<TypeList<HEAD, TAIL>, T>
{
    using Result = TypeList<HEAD, typename Append<TAIL, T>::Result>;
};

template <typename typelist, typename T>
struct Erase;
template <typename T>
struct Erase<NullType, T>
{
    using Result = NullType;
};
template <typename HEAD, typename TAIL>
struct Erase<TypeList<HEAD, TAIL>, HEAD>
{
    using Result = TAIL;
};

template <typename HEAD, typename TAIL, typename T>
struct Erase<TypeList<HEAD, TAIL>, T>
{
    using Result = TypeList<HEAD, typename Erase<TAIL, T>::Result>;
};

template <typename typelist, typename T>
struct EraseAll;
template <typename T>
struct EraseAll<NullType, T>
{
    using Result = NullType;
};
template <typename HEAD, typename TAIL>
struct EraseAll<TypeList<HEAD, TAIL>, HEAD>
{
    using Result = typename EraseAll<TAIL, HEAD>::Result;
};

template <typename HEAD, typename TAIL, typename T>
struct EraseAll<TypeList<HEAD, TAIL>, T>
{
    using Result = TypeList<HEAD, typename EraseAll<TAIL, T>::Result>;
};

template <typename typelsit, typename T, typename U>
struct Replace;
template <typename T, typename U>
struct Replace<NullType, T, U>
{
    using Result = NullType;
};
template <typename T, typename TAIL, typename U>
struct Replace<TypeList<T, TAIL>, T, U>
{
    using Result = TypeList<U, TAIL>;
};
template <typename HEAD, typename TAIL, typename T, typename U>
struct Replace<TypeList<HEAD, TAIL>, T, U>
{
    using Result = TypeList<HEAD, typename Replace<TAIL, T, U>::Result>;
};

template <typename typelist, typename T>
struct MostDerived;
template<typename T>
struct MostDerived<NullType, T>
{
    using Result = T;
};
template <typename HEAD, typename TAIL, typename T>
struct MostDerived<TypeList<HEAD, TAIL>, T>
{
private:
    using Candidate = typename MostDerived<TAIL, T>::Result;

public:
    using Result = typename Select<SUPERSUBCLASS(Candidate, HEAD), HEAD, Candidate>::Result;
};

template <typename typelist>
struct DerivedToFront;

template<>
struct DerivedToFront<NullType>
{
    using Result = NullType;
};
template <typename HEAD, typename TAIL>
struct DerivedToFront<TypeList<HEAD, TAIL>>
{
private:
    using TheMostDerived = typename MostDerived<TAIL, HEAD>::Result;
    using Temp = typename Replace<TAIL, TheMostDerived, HEAD>::Result;
public:
    using Result = TypeList<TheMostDerived, Temp>;
};

template <int N>
struct binary
{
    const static int value = binary<N/10>::value*2 + N%10;
};

template<>
struct binary<0>
{
    const static int value = 0;
};
} // namespace util

#endif