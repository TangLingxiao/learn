#ifndef __LOGMGR_H_
#define __LOGMGR_H_

#include <list>
#include <string>
#include <sstream>
#include <memory>
#include <mutex>
#include <thread>

enum LogLevel
{
	LEVEL_DEBUG,
	LEVEL_INFO,
	LEVEL_WARN,
	LEVEL_ERROR,
};

struct LogData
{
	LogData(const std::string& strLog, uint32_t time) :strLog(strLog), time(time) {}
	std::string strLog;
	uint32_t time;
};

class Log;

class LogMgr:public std::enable_shared_from_this<LogMgr>
{
public:
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

static std::shared_ptr<LogMgr> gLogMgr;

#define LOG_WITH_LEVELCHECK(level, x)do\
{\
	if(gLogMgr->getLogLevel()>level)\
	{\
		return;\
	}\
	std::ostringstream os;\
	os<<std::this_thread::get_id()<<"|"<<__FILE__<<":"<<__LINE__<<"|"<<x<<std::endl;\
	gLogMgr->addLog(os.str());\
}while(0)

#define LOG_DEBUG(x) LOG_WITH_LEVELCHECK(LEVEL_DEBUG, "DEBUG|" << x)
#define LOG_INFO(x) LOG_WITH_LEVELCHECK(LEVEL_INFO, "INFO|" << x)
#define LOG_WARN(x) LOG_WITH_LEVELCHECK(LEVEL_WARN, "WARN|" << x)
#define LOG_ERROR(x) LOG_WITH_LEVELCHECK(LEVEL_ERROR, "ERROR|" << x)
#endif
