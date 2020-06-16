#pragma once
#include "../Interface/ILogger.h"
#include <string>

class LogConsole
{
public:
    static void processLog(LogLevel level, const std::string &log);
};