#ifndef __COMMON_TIME_STOPWATCH_H__
#define __COMMON_TIME_STOPWATCH_H__
#include <chrono>
#include <random>
#include <algorithm>
#include <sstream>


template<typename T = std::chrono::milliseconds>
class StopWatch
{
public:
    StopWatch(){Reset();}
    inline void Reset(){ _tick = std::chrono::duration_cast<T>(std::chrono::system_clock::now().time_since_epoch()).count();}
    inline int64_t  Interval(){ return std::chrono::duration_cast<T>(std::chrono::system_clock::now().time_since_epoch()).count() - _tick;}
    void Printf( const char * content = nullptr){
		int64_t time = Interval();
        if (nullptr != content)
        {
            printf("%s interval time = %ld ms", content, time);
        }
        else
        {
			printf("interval time = %ld ms", time);
        }
    }
protected:
private:
    int64_t _tick;
};

struct PerfomanceInfo 
{
	int64_t maxTick;
	int64_t minTick;
	int64_t avrgTick;
	int64_t timoutCount;
};

template<typename T = std::chrono::milliseconds, int32_t TIME_OUT = 10>
class PerformanceWatch
{
	typedef std::vector<int64_t> TickPoints;
public:
	PerformanceWatch() { Begin(); }
	inline void Begin() { _tick = std::chrono::duration_cast<T>(std::chrono::system_clock::now().time_since_epoch()).count(); }
	inline int64_t  Interval() { return std::chrono::duration_cast<T>(std::chrono::system_clock::now().time_since_epoch()).count() - _tick; }
	inline void Check() {
		int64_t lastTick = _tick;
		_tick = std::chrono::duration_cast<T>(std::chrono::system_clock::now().time_since_epoch()).count();
		_tickPoints.push_back(_tick - lastTick);
	}

	std::string CreatePerformanceInfo()
	{
		int64_t maxTick = 0;
		int64_t minTick = 0;
		int64_t avrgTick = 0;
		int64_t timoutCount = 0;
		double total = 0;
		int64_t c99AvrgTick = 0;
		int64_t c99MaxTick = 0;

		if (_tickPoints.size() == 0)
			return "no date need create";

		std::sort(_tickPoints.begin(), _tickPoints.end());
		size_t size = _tickPoints.size();
		maxTick = _tickPoints[size - 1];
		minTick = _tickPoints[0];
		size_t c99Begin = size / (2 * 100);
		size_t c99End = size - c99Begin;
		double c99Total = 0;
		for (size_t i = 0; i < size; i++)
		{
			if (i >= c99Begin && i <= c99End)
				c99Total += _tickPoints[i];
			if (i == c99End)
				c99MaxTick = _tickPoints[i];
			total += _tickPoints[i];
			if (_tickPoints[i] >= TIME_OUT)
				timoutCount++;
		}
		avrgTick = total / size;
		c99AvrgTick = c99Total / (c99End - c99Begin);
		std::stringstream ostream;
		ostream << "Performance Info: maxTick:" << maxTick;
		ostream << ", minTick:" << minTick << ", avrgTick:" << avrgTick;
		ostream << ", Totoal(scaled-down1000):" << total / 1000 << ", c99MaxTick:" << c99MaxTick<<  ", c99avrgTick:" << c99AvrgTick;
		ostream << ", c99Totoal(scaled-down1000):" << c99Total / 1000 << ", TimeOut:" << timoutCount;
		return ostream.str();
	}
protected:
private:
	int64_t			_tick;
	int64_t			_totalTick;
	TickPoints		_tickPoints;
};


inline uint32_t  Random()
{
	static std::random_device dev;
	static std::default_random_engine eng(dev());
	static std::uniform_int_distribution<uint32_t> distribution;
	return distribution(eng);
}

inline int32_t GetRandom(int32_t nA, int32_t nB)
{
	if (nA == nB)return nA;

	if (nB < nA)std::swap(nA, nB);
	return nA + Random() % (nB - nA);
}

#endif