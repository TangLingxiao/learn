#pragma once

#include <stdint.h>
#include <string>
using namespace std;

const uint32_t SECOND_PER_MINUTE	= 60;
const uint32_t MINUTE_PER_HOUR		= 60;
const uint32_t SECOND_PER_HOUR		= 3600;
const uint32_t HOUR_PER_DAY			= 24;
const uint32_t SECOND_PER_DAY		= 86400;

class UtilTime
{
public:
	static int64_t getNow();
	static int64_t getNowMS();

	static string formatTime(int64_t iTime, const char *fmt = "%Y-%m-%d %H:%M:%S");
	static int64_t parseTime(const string &sTime, const char *fmt = "%Y-%m-%d %H:%M:%S");

	static uint32_t getDate(int64_t iTime, int32_t offset = 0);
	static uint32_t getHour(int64_t iTime, int32_t offset = 0);
	//@param iDate format: 20140820
	static uint32_t fromDate(uint32_t iDate, uint32_t iHour = 0);

	// get hour,min,sec, eg:230912
	static uint32_t getDayTime(int64_t iTime);

	// 取两个时间之间的间隔, 只会返回非负值
	static uint32_t timeElapse(int64_t iTime1, int64_t iTime2);
};

// 毫秒级定时器
class UtilTimer
{
public:
	UtilTimer(int64_t iInterval, int64_t iCurrTime)
		: m_iInterval(iInterval),
		m_iLastTriggerTime(iCurrTime)
	{
	}

	void reset(int64_t iInterval, int64_t iCurrTime);
	//检测定时器是否到时间
	bool operator()(int64_t iCurrTime);

private:
	bool isReady(int64_t iCurrTime);

private:
	int64_t m_iInterval;
	int64_t m_iLastTriggerTime;
};