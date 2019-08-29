#ifndef _SINGLETON_H_
#define _SINGLETON_H_

//c++11 thread safe
template<typename T>
class Sigleton
{
public:
	T& getInstance()
	{
		static T instance;
		return instance;
	}
};
#endif // !_SINGLETON_H_
