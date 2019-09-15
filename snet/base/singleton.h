#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template <typename T>
class Singleton
{
public:
    static T &getInstance()
    {
        static T instance;
        return instance;
    }
};
#endif