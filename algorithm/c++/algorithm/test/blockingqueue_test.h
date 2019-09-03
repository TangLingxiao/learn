#ifndef __BLOCKINGQUEUE_TEST_H__
#define __BLOCKINGQUEUE_TEST_H__

#include "../structure/blockingqueue.h"
#include "logmgr.h"
#include "../structure/countdownlatch.h"
#include <string>
#include <vector>
#include <memory>
#include <cstdio>

class BlockingQueueTest
{
public:
	BlockingQueueTest(int32_t num):m_latch(num), m_queue()
	{
		for (int32_t i = 0; i < num; ++i)
		{
			m_thread.emplace_back(new std::thread(std::bind(&BlockingQueueTest::threadFunc, this)));
		}
	}

	void run(int32_t times)
	{
		LOG_DEBUG("waiting for count down latch");
		m_latch.wait();
		LOG_DEBUG("all thread started");
		for (auto i = 0; i < times; ++i)
		{
			char buf[32];
			snprintf(buf, sizeof buf, "hello:%d", i);
			m_queue.put(buf);
			LOG_DEBUG("tid= " << std::this_thread::get_id() << ", put data " << buf << ", size = " << m_queue.size());
		}
	}

	void joinAll()
	{
		for (size_t i = 0; i < m_thread.size(); ++i)
		{
			m_queue.put("stop");
		}

		for (auto& thread : m_thread)
		{
			thread->join();
		}
	}

private:
	void threadFunc()
	{
		LOG_DEBUG("tid: " << std::this_thread::get_id() << " started");
		m_latch.countDown();
		bool running = true;
		while (running)
		{
			std::string res(m_queue.take());
			LOG_DEBUG("tid=" << std::this_thread::get_id() << ", get data " << res << ", size = " << m_queue.size());
			running = (res != "stop");
		}
		LOG_DEBUG("tid=" << std::this_thread::get_id() << ", stoped");
	}
	CountDownLatch m_latch;
	BlockingQueue<std::string> m_queue;
	std::vector<std::unique_ptr<std::thread>> m_thread;
};

void testBlockingQueue()
{
	BlockingQueueTest b(5);
	b.run(100);
	b.joinAll();
}

#endif
