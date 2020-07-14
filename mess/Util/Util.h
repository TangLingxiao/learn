#pragma once
#include <cassert>
#include <stdio.h>
#include <stdint.h>


#define SafeSprintf snprintf

#ifdef _DEBUG
static void __Lib_Assert(const char* file, int32_t line, const char* func, const char* debug) {
	fflush(stdout);
	fprintf(stderr,
		"\nAsssertion failed : \n=======assert string=======\nfile:%s\nline:%d\nfunction:%s\ndebug:%s\n===========================\n",
		file, line, func, debug);
	fflush(stderr);
	assert(false);
}

#define LIB_ASSERT(p, format, ...) { \
    char debug[4096]; \
    SafeSprintf(debug, sizeof(debug), format, ##__VA_ARGS__); \
    ((p) ? (void)0 : (void)__Lib_Assert(__FILE__, __LINE__, __FUNCTION__, debug)); \
}
#else
#define LIB_ASSERT(p, format, ...)
#endif