#ifndef __LOGMGR_H_
#define __LOGMGR_H_

#include <list>
#include <string>
#include <sstream>
#include <memory>
#include <mutex>
#include <thread>
#include "singleton.h"
enum LogLevel
{
	LEVEL_DEBUG,
	LEVEL_INFO,
	LEVEL_WARN,
	LEVEL_ERROR,
};

struct LogData
{
	LogData(const std::string& log, uint32_t time) :strLog(log), iTime(time) {}
	std::string strLog;
	uint32_t iTime;
};

class Log;

class LogMgr:public Singleton<LogMgr>
{
public:
	~LogMgr();
	bool init(const std::string& strName, const std::string& strPath);
	void release();
	void setLogLevel(LogLevel eLogLevel);
	void addLog(const std::string& strLog);
	LogLevel getLogLevel() { return m_eLevel; }
private:

	void loggerThreadEntry();
private:
	bool m_bTerminate = false;
	std::mutex m_mtx;
	std::string m_strPath = "./";
	LogLevel m_eLevel = LEVEL_DEBUG;
	std::shared_ptr<Log> m_pLog;
	std::list<LogData> m_lLog;
	std::thread m_thread;
};

#define LOG_WITH_LEVELCHECK(level, x)do\
{\
	if(LogMgr::getInstance().getLogLevel() <= level)\
	{\
		std::ostringstream os;\
		os<<std::this_thread::get_id()<<"|"<<__FILE__<<":"<<__LINE__<<"|"<<x<<std::endl;\
		LogMgr::getInstance().addLog(os.str());\
	}\
}while(0)

#define LOG_DEBUG(x) LOG_WITH_LEVELCHECK(LEVEL_DEBUG, "DEBUG|" << x)
#define LOG_INFO(x) LOG_WITH_LEVELCHECK(LEVEL_INFO, "INFO|" << x)
#define LOG_WARN(x) LOG_WITH_LEVELCHECK(LEVEL_WARN, "WARN|" << x)
#define LOG_ERROR(x) LOG_WITH_LEVELCHECK(LEVEL_ERROR, "ERROR|" << x)
#endif
