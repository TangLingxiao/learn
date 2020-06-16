
#include "TimeHelper.h"
#include <ctime>
#include <sys/time.h>
#include <string.h>
#include <stdexcept>

int64_t UtilTime::getNow()
{
	return time(0);
}

int64_t UtilTime::getNowMS()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (int64_t)tv.tv_sec * 1000 + (int64_t)tv.tv_usec / 1000;
}

string UtilTime::formatTime(int64_t iTime, const char *fmt)
{
	struct tm tmnow;
	time_t t = iTime;
	localtime_r(&t, &tmnow);

	char buf[256];
	size_t len = strftime(buf, sizeof(buf), fmt, &tmnow);
	buf[len >= sizeof(buf) ? sizeof(buf) - 1 : len] = 0;
	return buf;
}

int64_t UtilTime::parseTime(const string &sTime, const char *fmt)
{
	struct tm stTm;
	memset(&stTm, 0, sizeof(stTm));
	if (strptime(sTime.c_str(), fmt, &stTm) == NULL)
	{
		//throw std::runtime_error("invalid time: " + sTime + ", format: " + fmt);
		return -1;
	}
	stTm.tm_isdst = -1;
	return mktime(&stTm);
}

uint32_t UtilTime::getDate(int64_t iTime, int32_t offset)
{
	time_t now_time = iTime + (offset * 86400);
	struct tm tmnow;
	localtime_r(&now_time, &tmnow);
	return (tmnow.tm_year + 1900) * 10000 + (tmnow.tm_mon + 1) * 100 + tmnow.tm_mday;
}

uint32_t UtilTime::getHour(int64_t iTime, int32_t offset)
{
	time_t now_time = iTime + (offset * 3600);
	struct tm tmnow;
	localtime_r(&now_time, &tmnow);
	return tmnow.tm_hour;
}

uint32_t UtilTime::fromDate(uint32_t iDate, uint32_t iHour)
{
	struct tm tmnow;
	memset(&tmnow, 0, sizeof(tmnow));
	tmnow.tm_year = iDate / 10000 - 1900;
	tmnow.tm_mon = iDate / 100 % 100 - 1;
	tmnow.tm_mday = iDate % 100;
	tmnow.tm_hour = iHour;
	tmnow.tm_isdst = -1;
	return mktime(&tmnow);
}

uint32_t UtilTime::getDayTime(int64_t iTime)
{
	time_t now_time = iTime;
	struct tm tmnow;
	localtime_r(&now_time, &tmnow);
	return tmnow.tm_hour*10000 + tmnow.tm_min*100 + tmnow.tm_sec;
}

uint32_t UtilTime::timeElapse(int64_t iTime1, int64_t iTime2)
{
	if (iTime1 >= iTime2)
	{
		return iTime1 - iTime2;
	}
	return 0;
}

void UtilTimer::reset(int64_t iInterval, int64_t iCurrTime)
{
	m_iInterval = iInterval;
	m_iLastTriggerTime = iCurrTime;
}

bool UtilTimer::operator()(int64_t iCurrTime)
{
	return isReady(iCurrTime);
}

bool UtilTimer::isReady(int64_t iCurrTime)
{
	if (m_iLastTriggerTime + m_iInterval > iCurrTime)
	{
		return false;
	}

	// 避免超时时间较长时连续触发多次
	if(iCurrTime > m_iLastTriggerTime + m_iInterval + m_iInterval)
	{
		m_iLastTriggerTime = iCurrTime;
	}
	else
	{
		m_iLastTriggerTime += m_iInterval;
	}
	return true;

}
