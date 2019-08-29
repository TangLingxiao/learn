#include "logmgr.h"
#include <thread>
#include <mutex>
#include <list>
#include <chrono>
#include <cstdio>
#include <cstdlib>

class Log
{
public:
	Log(const std::string& strFileName, const std::string& strFilePath) :m_strFileName(strFileName), m_strFilePath(strFilePath)
	{
		if (m_strFileName.empty())
		{
			m_strFileName = "default.log";
		}
		m_strTrueFile = m_strFilePath + '/' + m_strFileName;
	}
	virtual ~Log()
	{
	}
	
	bool init()
	{
		auto fp = fopen(m_strTrueFile.c_str(), "a+");
		if (nullptr == fp)
		{
			return false;
		}
		m_fp.reset(fp); //std::make_shared<std::FILE>(fp);
		return true;
	}

	void writeLog(const LogData& sLogData)
	{
		uint64_t n = 0;
		char buf[64];
		time_t time = sLogData.time;
		size_t len = strftime(buf, 64, "%Y-%m-%d %H:%M:%S", localtime(&time));
		buf[len < 64 ? len : 63] = '\0';
		std::string strTime(buf);
		n += fwrite(strTime.c_str(), 1, strTime.size(), m_fp.get());
		n += fwrite(sLogData.strLog.c_str(), 1, sLogData.strLog.size(), m_fp.get());
		m_iFileLength += n;
		fflush(m_fp.get());
	}

private:
	std::string m_strFileName;
	std::string m_strFilePath;
	std::string m_strTrueFile;
	//std::FILE* m_fp = nullptr;
	std::shared_ptr<std::FILE> m_fp;
	uint64_t m_iFileLength = 0;
};

bool LogMgr::init(const std::string & strName, const std::string & strPath)
{
	std::lock_guard<std::mutex> lock(m_mtx);
	m_strPath = strPath;
	m_pLog.reset(new Log(strName, strPath));
	if (!m_pLog->init())
	{
		return false;
	}
	m_thread = std::thread(std::bind(&LogMgr::loggerThreadEntry, shared_from_this()));
	return true;
}

void LogMgr::release()
{
	m_bTerminate = true;
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}

void LogMgr::setLogLevel(LogLevel eLogLevel)
{
	m_eLevel = eLogLevel;
}

void LogMgr::addLog(const std::string & strLog)
{
	if (m_bTerminate)
	{
		return;
	}
	uint32_t iNow = static_cast<uint32_t>(time(0));
	std::lock_guard<std::mutex> lock(m_mtx);
	m_lLog.emplace_back(strLog, iNow);
}

void LogMgr::loggerThreadEntry()
{
	while (true)
	{
		std::list<LogData> lLog;
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			lLog.swap(m_lLog);
		}
		if (lLog.empty())
		{
			std::lock_guard<std::mutex> lock(m_mtx);
			if (m_bTerminate)
			{
				break;
			}
			std::this_thread::sleep_for(std::chrono::duration<int32_t>(1));
			continue;
		}
		for (auto& log : lLog)
		{
			m_pLog->writeLog(log);
		}
	}
}

