#ifndef __COUNTDOWN_LATCH_H__
#define __COUNTDOWN_LATCH_H__

#include <mutex>
#include <condition_variable>

class CountDownLatch
{
public:
	explicit CountDownLatch(int32_t iCnt):m_mtx(), m_iCnt(iCnt), m_cond(){}
	
	int32_t getCount()
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		return m_iCnt;
	}

	void countDown()
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		--m_iCnt;
		if (m_iCnt == 0)
		{
			m_cond.notify_all();
		}
	}

	void wait()
	{
		std::unique_lock<std::mutex> lock(m_mtx);
		while (m_iCnt > 0)
		{
			m_cond.wait(lock);
		}
	}
private:
	std::mutex m_mtx;
	int32_t m_iCnt;
	std::condition_variable m_cond;
};

#endif // !__COUNTDOWN_LATCH__
