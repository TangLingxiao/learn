#pragma once

#include "ICore.h"

enum LogLevel
{
    LEVEL_TRACE,
    LEVEL_DEBUG,
    LEVEL_INFO,
    LEVEL_WARN,
    LEVEL_ERROR,
    LEVEL_MAX,
};

enum LogType
{
    TYPE_ROLL,
    TYPE_DAY,
    TYPE_HOUR,
};

class ILogger : public ICore
{
public:
    virtual ~ILogger() = default;
};