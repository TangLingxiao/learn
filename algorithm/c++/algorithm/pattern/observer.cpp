#include "observer.h"

#include "logmgr.h"
void Observerable::register_(const std::weak_ptr<Observer>& observer)
{
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		m_vObservers.emplace_back(observer);
	}
	LOG_DEBUG("registerd");
}

void Observerable::notify()
{
	std::lock_guard<std::mutex> lock(m_mtx);
	for (auto it = m_vObservers.begin(); it != m_vObservers.end();)
	{
		auto shared = it->lock();
		if (shared)
		{
			shared->update();
			++it;
		}
		else
		{
			it = m_vObservers.erase(it);
		}
	}
}

void Observer::update()
{
	LOG_INFO("notiified");
}
