#pragma once
#include <map>
#include <vector>
#include <functional>
#include <set>
#include <cassert>
#include <string>

#define EVENT_CALLBACK(cls_func, obj) obj, std::bind(&cls_func, obj, std::placeholders::_1), __FILE__, __LINE__

enum EEventDefine
{
	
};

//内部事件的基类
class CEvent
{
public:
	CEvent(EEventDefine eEventType);
	CEvent() = default;
	virtual ~CEvent();
	inline EEventDefine GetEventType() const { return eEventType; }
protected:
	EEventDefine eEventType;
};
