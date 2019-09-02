#ifndef _SINGLETON_H_
#define _SINGLETON_H_

//c++11 thread safe
template<typename T>
class Singleton
{
public:
	static T& getInstance()
	{
		static T instance;
		return instance;
	}
};

/*
#include <pthread.h>

template <typename T>
class Singleton
{
public:
	static T& Instance()
	{
		pthread_once(&ponce, init);
		return *pInstace;
	}

private:
	static void init()
	{
		pInstace = new T();
	}

	static T* pInstace;
	static pthread_once_t ponce;
};

*/
#endif // !_SINGLETON_H_
