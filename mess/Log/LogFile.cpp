#include "LogFile.h"
#include "../Util/TimeHelper.h"
#include "../Util/FileHelper.h"
void Log::writeLog(const LogData &data)
{
    if (prepareLogFile(data))
    {
        uint64_t n = 0;
        std::string time = UtilTime::formatTime(data._time, "%Y-%m-%d %H:%M:%S|");
        n += fwrite(time.c_str(), 1, time.size(), _fp);
        n += fwrite(data._log.c_str(), 1, data._log.size(), _fp);
        _filesize += n;
        fflush(_fp);
    }
}

RollLog::~RollLog()
{
    if (_fp)
    {
        fclose(_fp);
        _fp = nullptr;
    }
}

void RollLog::setRollLogInfo(uint32_t maxfile, uint64_t maxsize)
{
    _maxfile = std::max<uint32_t>(1, maxfile);
    _maxsize = std::max<uint64_t>(128, maxsize);
}

std::string RollLog::getRollLogFileName(uint32_t index)
{
    if (index == 0)
    {
        return getLogFilePathPrefix() + ".log";
    }
    return getLogFilePathPrefix() + std::to_string(index) + ".log";
}

bool RollLog::prepareLogFile(const LogData &data)
{
    if (_fp == nullptr)
    {
        //try to append to existing log
        std::string curRollFile = getRollLogFileName(0);
        _fp = fopen(curRollFile.c_str(), "a+");
        if (_fp == nullptr)
        {
            return false;
        }
        fseeko(_fp, 0, SEEK_END);
        _filesize = ftello(_fp);
    }
    if (_filesize < _maxsize)
    {
        return true;
    }
    fclose(_fp);
    _fp = nullptr;
    _filesize = 0;

    //roll log file
    UtilFile::removeFile(getRollLogFileName(_maxfile - 1));
    for (int i = _maxfile - 2; i >= 0; --i)
    {
        UtilFile::moveFile(getRollLogFileName(i), getRollLogFileName(i + 1));
    }
    return prepareLogFile(data);
}

DayLog::~DayLog()
{
    for (auto &it : _datefile)
    {
        auto *fp = it.second._fp;
        if (fp != nullptr)
        {
            fclose(fp);
        }
    }
    _fp = nullptr;
    _datefile.clear();
}

void DayLog::closeExpiredFile()
{
    if (_datefile.empty())
    {
        return;
    }
    int64_t iNow = UtilTime::getNow();
    int64_t iExpireTime = iNow - 60;
    for (auto it = _datefile.begin(); it != _datefile.end();)
    {
        auto &info = it->second;
        if (info._accesstime < iExpireTime)
        {
            if (info._fp != nullptr)
            {
                fclose(info._fp);
            }
            it = _datefile.erase(it);
            continue;
        }
        ++it;
    }
}

bool DayLog::prepareLogFile(const LogData &data)
{
    uint32_t date = UtilTime::getDate(data._time);
    auto it = _datefile.find(date);
    if (it != _datefile.end())
    {
        auto &info = it->second;
        info._accesstime = UtilTime::getNow();
        _fp = info._fp;
        return true;
    }
    auto curDayFile = getLogFilePathPrefix() + "_" + std::to_string(date) + ".log";
    _fp = fopen(curDayFile.c_str(), "a+");
    if (_fp == nullptr)
    {
        return false;
    }
    auto &info = _datefile[date];
    info._fp = _fp;
    info._accesstime = UtilTime::getNow();
    return true;
}

bool HourLog::prepareLogFile(const LogData &data)
{
    uint32_t date = UtilTime::getDate(data._time) + UtilTime::getHour(data._time);
    auto it = _datefile.find(date);
    if (it != _datefile.end())
    {
        auto &info = it->second;
        info._accesstime = UtilTime::getNow();
        _fp = info._fp;
        return true;
    }
    char buf[32];
    snprintf(buf, sizeof(buf), "%d_%d", date / 100, date % 100);
    auto curDayFile = getLogFilePathPrefix() + "_" + buf + ".log";
    _fp = fopen(curDayFile.c_str(), "a+");
    if (_fp == nullptr)
    {
        return false;
    }
    auto &info = _datefile[date];
    info._fp = _fp;
    info._accesstime = UtilTime::getNow();
    return true;
}