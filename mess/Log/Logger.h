#pragma once

#include "../Pattern/Singleton.h"
#include "../Interface/ILogger.h"
#include "LogFile.h"
#include <mutex>
#include <list>
#include <thread>
#include <map>
#include <sstream>

class Log;
struct LogData;

class Logger : public ILogger, public Singleton<Logger>
{
public:
    virtual bool ready() override;
    virtual bool init() override;
    virtual bool destroy() override;

    void setLogInfo(const std::string &path, const std::string &logprefix, const std::string &errorprefix = "");
    void setRollLogInfo(Log *log, uint32_t maxfile, uint64_t maxsize);
    void setLogLevel(LogLevel level);
    void setLogLevel(const std::string &level);

    Log *getRollLog(const std::string &filename = "");
    Log *getDayLog(const std::string &filename, LogType type = TYPE_DAY);
    Log *getHourLog(const std::string &filename, LogType type = TYPE_HOUR);
    bool checkLogLevel(LogLevel level);
    LogType getLogType(Log *log);
    const std::string &getLogFileName(Log *log);
    void addLog(Log *log, const std::string &data);
    void printConsoleLog(LogLevel level, const std::string &data);

private:
    void loggerThreadEntry();
    Log *getInitLogByType(const std::string &filename, LogType type);
    void cleanup();

private:
    bool _terminate{false};
    bool _logEnabled{false};

    std::string _path{"./"};
    std::string _logprefix;
    std::string _errorprefix;
    LogLevel _level{LEVEL_DEBUG};
    std::list<LogData> _logdata;
    std::map<std::string, Log *> _allLog;
    std::mutex _mtx;
    std::thread _thread;
};

#define LOG_MGR Logger::getInstance()
const char *briefFileName(const char *name);

#define LOG_WITH_LEVELCHECK(level, x)                                               \
    do                                                                              \
    {                                                                               \
        Log *log = LOG_MGR.getRollLog();                                            \
        if (log && LOG_MGR.checkLogLevel(level))                                    \
        {                                                                           \
            std::ostringstream oss;                                                 \
            oss << std::this_thread::get_id() << "|" << briefFileName(__FILE__)     \
                << ":" << __LINE__ << ":" << __FUNCTION__ << "|" << x << std::endl; \
            LOG_MGR.printConsoleLog(level, oss.str());                              \
            LOG_MGR.addLog(log, oss.str());                                         \
        }                                                                           \
    } while (0)

#define DAYLOG(file, x)                     \
    do                                      \
    {                                       \
        Log *log = LOG_MGR.getDayLog(file); \
        if (log)                            \
        {                                   \
            std::ostringstream oss;         \
            oss << x << std::endl;          \
            LOG_MGR.addLog(log, oss.str()); \
        }                                   \
    } while (0)

#define HOURLOG(file, x)                     \
    do                                       \
    {                                        \
        Log *log = LOG_MGR.getHourLog(file); \
        if (log)                             \
        {                                    \
            std::ostringstream oss;          \
            oss << x << std::endl;           \
            LOG_MGR.addLog(log, oss.str());  \
        }                                    \
    } while (0)

#define LOG_DEBUG(x) LOG_WITH_LEVELCHECK(LEVEL_DEBUG, "DEBUG|" << x)
#define LOG_INFO(x) LOG_WITH_LEVELCHECK(LEVEL_INFO, "INFO|" << x)
#define LOG_WARN(x) LOG_WITH_LEVELCHECK(LEVEL_WARN, "WARN|" << x)
#define LOG_ERROR(x)                                                                                       \
    do                                                                                                     \
    {                                                                                                      \
        LOG_WITH_LEVELCHECK(LEVEL_ERROR, "ERROR|" << x);                                                   \
        DAYLOG("error", std::this_thread::get_id() << "|" << briefFileName(__FILE__)                       \
                                                   << ":" << __LINE__ << ":" << __FUNCTION__ << "|" << x); \
    } while (0)
