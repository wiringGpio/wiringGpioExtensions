#pragma once
#include <wiringGpioExtensions.h>


//  Log level
extern wiringGpioLogLevel LogLevel;

void WriteLogToConsole(wiringGpioLogEvent log);

void AddLog(wiringGpioLogLevel level, const char* sender, const char* function, const char* data);

void AddLogFormatted(wiringGpioLogLevel level, const char* sender, const char* function, const char* format, ...);