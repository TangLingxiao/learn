#pragma once
#include <functional>
#include <map>
#include <set>
//#include <unordered_map>
#include "Event.h"

class CEventDispatcher
{
public:
	CEventDispatcher();
	~CEventDispatcher();

	bool Init();
	void UnInit();

public:
	using ListenerCallback = std::function<void(CEvent* pEvent)>;
	struct EventDispatchEntry
	{
		EventDispatchEntry() = default;
		EventDispatchEntry(const char* name, int32_t line, ListenerCallback call, bool own)
			: file_name(name)
			, line_no(line)
			, callback(call)
			, is_own(own)
		{}
		const char* file_name{ nullptr };
		int32_t line_no{ 0 };
		ListenerCallback callback{ nullptr };
		bool is_own{ true };
	};
	using ListenerInfoMap = std::map<const void*, EventDispatchEntry *>;

	void AddEventListener(EEventDefine eEventType, const void* obj, const ListenerCallback& callback, const char* file_name, int32_t line_no, bool own = true);
	void RemoveEventListener(EEventDefine eEventType, const void* obj, const ListenerCallback& callback, const char* file_name = nullptr, int32_t line_no = 0);
	void DispatcherEvent(CEvent* pEvent);

private:
	std::set<uint32_t> event_lock_;
	//std::unordered_map<uint32_t, ListenerInfoMap> hash_event_handler_;
	std::map<uint32_t, ListenerInfoMap> hash_event_handler_;
};
