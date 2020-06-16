#include "Logger.h"
#include "LogFile.h"
#include "LogConsole.h"
#include "../Util/FileHelper.h"
#include "../Util/StringHelper.h"
#include "../Util/TimeHelper.h"

const char *briefFileName(const char *name)
{
    if (name[0] != '/')
    {
        return name;
    }
    const char *p = strrchr(name, '/');
    if (p != NULL)
    {
        return p + 1;
    }
    return name;
}

bool Logger::ready()
{
    return true;
}

bool Logger::init()
{
    return true;
}

bool Logger::destroy()
{
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (_logEnabled)
        {
            _logEnabled = false;
            _terminate = true;
        }
    }
    if (_thread.joinable())
    {
        _thread.join();
    }
    return true;
}

void Logger::setLogInfo(const std::string &path, const std::string &logprefix, const std::string &errorprefix)
{
    std::lock_guard<std::mutex> lock(_mtx);
    if (!_logEnabled && !logprefix.empty())
    {
        _path = UtilFile::getAbsolutePath(path);
        UtilFile::makeDirectoryRecursive(_path);
        //atexit(&Logger::destroy);
        _errorprefix = errorprefix;
        _logprefix = logprefix;
        _logEnabled = true;
        _thread = std::thread(std::bind(&Logger::loggerThreadEntry, this));
    }
}

void Logger::setRollLogInfo(Log *log, uint32_t maxfile, uint64_t maxsize)
{
    if (log)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (log->getLogType() == TYPE_ROLL)
        {
            RollLog *rolllog = static_cast<RollLog *>(log);
            rolllog->setRollLogInfo(maxfile, maxsize);
        }
    }
}

void Logger::setLogLevel(LogLevel level)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _level = level;
}

void Logger::setLogLevel(const std::string &slevel)
{
    LogLevel level{LEVEL_DEBUG};
    std::string s = UtilString::toupper(slevel);
    if (s == "DEBUG")
    {
        level = LEVEL_DEBUG;
    }
    else if (s == "INFO")
    {
        level = LEVEL_INFO;
    }
    else if (s == "WARN")
    {
        level = LEVEL_WARN;
    }
    else if (s == "ERROR")
    {
        level = LEVEL_ERROR;
    }
    setLogLevel(level);
}

Log *Logger::getInitLogByType(const std::string &filename, LogType type)
{
    std::lock_guard<std::mutex> lock(_mtx);
    if (!_logEnabled)
    {
        return nullptr;
    }
    if (filename.find('/') != std::string::npos)
    {
        return nullptr;
    }
    auto it = _allLog.find(filename);
    if (it != _allLog.end())
    {
        if (it->second->getLogType() != type)
        {
            return nullptr;
        }
        return it->second;
    }
    Log *log{nullptr};
    if (type == TYPE_ROLL)
    {
        log = new RollLog(_path, _logprefix, filename, type);
    }
    else if (type == TYPE_DAY)
    {
        log = new DayLog(_path, _logprefix, filename, type);
    }
    else if (type == TYPE_HOUR)
    {
        log = new HourLog(_path, _logprefix, filename, type);
    }
    else
    {
        return nullptr;
    }
    _allLog[filename] = log;
    return log;
}

Log *Logger::getRollLog(const std::string &filename)
{
    return getInitLogByType(filename, TYPE_ROLL);
}

Log *Logger::getDayLog(const std::string &filename, LogType type)
{
    return getInitLogByType(filename, type);
}

Log *Logger::getHourLog(const std::string &filename, LogType type)
{
    return getInitLogByType(filename, type);
}

bool Logger::checkLogLevel(LogLevel level)
{
    return level >= _level;
}

LogType Logger::getLogType(Log *log)
{
    if (log)
    {
        return log->getLogType();
    }
    return TYPE_ROLL;
}

const std::string &Logger::getLogFileName(Log *log)
{
    if (log)
    {
        return log->getFileName();
    }
    return UtilString::getEmptyString();
}

void Logger::addLog(Log *log, const std::string &data)
{
    if (log)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (!_logEnabled)
        {
            return;
        }
        _logdata.emplace_back(log, data, UtilTime::getNow());
    }
}

void Logger::printConsoleLog(LogLevel level, const std::string &data)
{
    LogConsole::processLog(level, data);
}

void Logger::loggerThreadEntry()
{
    int64_t lasttick = UtilTime::getNow();
    while (true)
    {
        std::list<LogData> data;
        {
            std::lock_guard<std::mutex> lock(_mtx);
            if (!_allLog.empty())
            {
                int64_t now = UtilTime::getNow();
                if (lasttick + 10 < now)
                {
                    for (auto &it : _allLog)
                    {
                        it.second->closeExpiredFile();
                    }
                    lasttick = now;
                }
            }
            std::swap(data, _logdata);
        }
        if (data.empty())
        {
            if (_terminate)
            {
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        for (auto &log : data)
        {
            log._logger->writeLog(log);
        }
    }
    cleanup();
}

void Logger::cleanup()
{
    std::lock_guard<std::mutex> lock(_mtx);
    for (auto &it : _allLog)
    {
        auto *log = it.second;
        if (log)
        {
            delete log;
            log = nullptr;
        }
    }
    _allLog.clear();
    _logdata.clear();
}