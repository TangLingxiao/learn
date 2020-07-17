#pragma once
#include <cassert>
#include <stdio.h>
#include <stdint.h>

#define SafeSprintf snprintf

#ifdef _DEBUG
static void __Lib_Assert(const char *file, int32_t line, const char *func, const char *debug)
{
	fflush(stdout);
	fprintf(stderr,
			"\nAsssertion failed : \n=======assert string=======\nfile:%s\nline:%d\nfunction:%s\ndebug:%s\n===========================\n",
			file, line, func, debug);
	fflush(stderr);
	assert(false);
}

#define LIB_ASSERT(p, format, ...)                                                     \
	{                                                                                  \
		char debug[4096];                                                              \
		SafeSprintf(debug, sizeof(debug), format, ##__VA_ARGS__);                      \
		((p) ? (void)0 : (void)__Lib_Assert(__FILE__, __LINE__, __FUNCTION__, debug)); \
	}
#else
#define LIB_ASSERT(p, format, ...) 
#endif

/// usage: 两个已排序集合 s1, s2, 将仅存在于 s1 的元素放进 out1， 仅存在于 s2 的元素放进 out2
/// @first1: s1.being()
/// @last1: s1.end()
/// @first2: s2.begin()
/// @last2: s2.end()
/// @out1:
/// @out2:
template <class InputIterator1, class InputIterator2, class OutputIterator1, class OutputIterator2>
void Difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator1 out1, OutputIterator2 out2)
{
	while (first1 != last1 || first2 != last2)
	{
		if (first1 == last1)
		{
			while (first2 != last2)
			{
				*out2 = *first2;
				++out2;
				++first2;
			}
			return;
		}
		if (first2 == last2)
		{
			while (first1 != last1)
			{
				*out1 = *first1;
				++out1;
				++first1;
			}
			return;
		}
		if (*first1 < *first2)
		{
			*out1 = *first1;
			++out1;
			++first1;
		}
		else if (*first2 < *first1)
		{
			*out2 = *first2;
			++out2;
			++first2;
		}
		else
		{
			++first1;
			++first2;
		}
	}
}

/// 同上，但是把 s1 s2 的交集放进 inter
template <class InputIterator1, class InputIterator2, class OutputIterator1, class OutputIterator2, class InterIterator>
void DifferenceAndInter(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator1 out1, OutputIterator2 out2, InterIterator inter)
{
	while (first1 != last1 || first2 != last2)
	{
		if (first1 == last1)
		{
			while (first2 != last2)
			{
				*out2 = *first2;
				++out2;
				++first2;
			}
			return;
		}
		if (first2 == last2)
		{
			while (first1 != last1)
			{
				*out1 = *first1;
				++out1;
				++first1;
			}
			return;
		}
		if (*first1 < *first2)
		{
			*out1 = *first1;
			++out1;
			++first1;
		}
		else if (*first2 < *first1)
		{
			*out2 = *first2;
			++out2;
			++first2;
		}
		else
		{
			*inter = *first1;
			++first1;
			++first2;
		}
	}
}