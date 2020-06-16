#pragma once

#include <stdio.h>
#include <string>
#include <map>
#include "../Interface/ILogger.h"

class Log;
struct LogData
{
    LogData(Log *logger, const std::string &log, int64_t time) : _logger(logger), _log(log), _time(time) {}
    Log *_logger;
    std::string _log;
    int64_t _time;
};

struct LogFileInfo
{
    FILE *_fp;
    int64_t _accesstime;
};

class Log
{
public:
    Log(const std::string &path, const std::string &filenameprefix, const std::string &filename, LogType type) : _path(path), _filename(filename), _type(type), _filesize(0), _fp(nullptr)
    {
        if (_path.empty())
        {
            _path = ".";
        }
        _pathprefix = _path + "/" + filenameprefix;
        if (!_filename.empty())
        {
            _pathprefix += "_" + _filename;
        }
    }

    virtual ~Log() {}

    LogType getLogType() { return _type; }
    const std::string &getFileName() { return _filename; }

    void writeLog(const LogData &data);

    virtual void closeExpiredFile(){}
protected:
    const std::string &getLogFilePathPrefix() { return _pathprefix; }
    virtual bool prepareLogFile(const LogData &data) = 0;

protected:
    std::string _path;
    std::string _filename;
    std::string _pathprefix;
    LogType _type;
    uint64_t _filesize;
    FILE *_fp;
};

class RollLog : public Log
{
public:
    RollLog(const std::string &path, const std::string &filenameprefix, const std::string &filename, LogType type) : Log(path, filenameprefix, filename, type), _maxfile(10), _maxsize(100 * 1024 * 1024) {}
    virtual ~RollLog();
    void setRollLogInfo(uint32_t maxfile, uint64_t maxsize);

protected:
    std::string getRollLogFileName(uint32_t index);
    virtual bool prepareLogFile(const LogData &data) override;

private:
    uint32_t _maxfile;
    uint64_t _maxsize;
};

class DayLog : public Log
{
public:
    DayLog(const std::string &path, const std::string &filenameprefix, const std::string &filename, LogType type) : Log(path, filenameprefix, filename, type) {}
    virtual ~DayLog();
    virtual void closeExpiredFile();

protected:
    bool prepareLogFile(const LogData &data)override;

protected:
    std::map<uint32_t, LogFileInfo> _datefile;
};

class HourLog : public DayLog
{
public:
    HourLog(const std::string &path, const std::string &filenameprefix, const std::string &filename, LogType type) : DayLog(path, filenameprefix, filename, type) {}

protected:
    bool prepareLogFile(const LogData &data) override;
};