#include "EventDispatcher.h"
#include "Pool/ObjectPool.h"

CEventDispatcher::CEventDispatcher()
{
}

CEventDispatcher::~CEventDispatcher()
{
}

bool CEventDispatcher::Init()
{
	return true;
}

void CEventDispatcher::UnInit()
{
	for (auto& it_info_map : hash_event_handler_)
	{
		auto& it_listener = it_info_map.second;
		auto it = it_listener.begin();
		while (it != it_listener.end())
		{
			DELETE_OBJECT(it->second);
			it = it_listener.erase(it);
		}
	}
	hash_event_handler_.clear();
}

void CEventDispatcher::AddEventListener(EEventDefine eEventType, const void* obj, const ListenerCallback& callback, 
	const char* file_name, int32_t line_no, bool own/* = true*/)
{
	auto& info_map = hash_event_handler_[eEventType];
	auto it_entry = info_map.find(obj);
	auto* entry = NEW_OBJECT(EventDispatchEntry, file_name, line_no, callback, own);
	if (it_entry == info_map.end())
	{
		info_map.emplace(obj, entry);
	}
	else
	{
		DELETE_OBJECT(it_entry->second);
		it_entry->second = entry;
	}
}

void CEventDispatcher::RemoveEventListener(EEventDefine eEventType, const void* obj, const ListenerCallback& callback, const char* file_name/* = nullptr*/, int32_t line_no/* = 0*/)
{
	(void)callback;(void)file_name;(void)line_no;
	auto it_info_map = hash_event_handler_.find(eEventType);
	if (it_info_map == hash_event_handler_.end())
	{
		return;
	}
	auto& info_map = it_info_map->second;
	auto it_entry = info_map.find(obj);
	if (it_entry == info_map.end())
	{
		return;
	}
	DELETE_OBJECT(it_entry->second);
}

void CEventDispatcher::DispatcherEvent(CEvent* pEvent)
{
	if (!pEvent)
	{
		return;
	}
	auto it_info_map = hash_event_handler_.find(pEvent->GetEventType());
	if (it_info_map == hash_event_handler_.end())
	{
		DELETE_OBJECT(pEvent);
		return;
	}

	auto result = event_lock_.emplace(pEvent->GetEventType());
	auto& it_listener = it_info_map->second;

	if (result.second)
	{
		auto it = it_listener.begin();
		while (it != it_listener.end())
		{
			auto* entry_ptr = it->second;
			if (entry_ptr && entry_ptr->callback)
			{
				(entry_ptr->callback)(pEvent);
				++it;
			}
			else
			{
				it = it_listener.erase(it);
			}
		}
		event_lock_.erase(pEvent->GetEventType());
	}
	else
	{
		for (const auto& it : it_listener)
		{
			auto* entry_ptr = it.second;
			if (entry_ptr && entry_ptr->callback)
			{
				(entry_ptr->callback)(pEvent);
			}
		}
	}

	DELETE_OBJECT(pEvent);
}
