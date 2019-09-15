#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

#include <mutex>
#include <thread>
#include <condition_variable>
#include <deque>

template<typename T>
class BlockingQueue
{
public:
	void put(const T& x)
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		m_queue.push_back(x);
		m_cond.notify_one();
	}

	void put(T && x)
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		m_queue.emplace_back(std::move(x));
		m_cond.notify_one();
	}
	
	T take()
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		while (m_queue.empty())
		{
			m_cond.wait(lock);
		}
		T front(std::move(m_queue.front()));
		m_queue.pop_front();
		return std::move(front);
	}

	size_t size()
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		return m_queue.size();
	}
private:
	std::mutex m_mtx;
	std::condition_variable m_cond;
	std::deque<T> m_queue;
};


#endif // !__BLOCKING_QUEUE_H__
