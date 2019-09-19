#ifndef __CALLBACK_H__
#define __CALLBACK_H__
#include <functional>
using TimerCallBack = std::function<void()>;
using EventCallback = std::function<void()>;
#endif