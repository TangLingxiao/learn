#include "LogConsole.h"
#include <stdio.h>


enum Color { 
    BLACK,
    RED,
    GREEN,
    BROWN,
    BLUE,
    MAGENTA,
    CYAN,
    GRAY,
    DARK_GRAY,
    LIGHT_RED,
    LIGHT_GREEN,
    YELLOW,
    LIGHT_BLUE,
    LIGHT_MAGENTA,
    LIGHT_CYAN,
    WHITE
};

const char *ColorSeq[] = {
    "22;30",
    "22;31",
    "22;32",
    "22;33",
    "22;34",
    "22;35",
    "22;36",
    "22;37",
    "01;30",
    "01;31",
    "01;32",
    "01;33",
    "01;34",
    "01;35",
    "01;36",
    "01;37"
};

static Color gLogColorArray[LEVEL_MAX] = {
	CYAN,		 //DEBUG
    GREEN,       //INFO
	YELLOW,      //WARN
	RED,         //ERROR
	//MAGENTA,     //FATAL
};


void LogConsole::processLog(LogLevel level, const std::string &log)
{
    fprintf(stderr, "\033[%sm%s\n\033[0m", ColorSeq[gLogColorArray[level]], log.c_str());
}