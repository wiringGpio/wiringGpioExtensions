#pragma once
#include <wiringGpioExtensions.h>


//  Log level
extern wiringGpioLogLevel LoggingLevel;

void WriteLogToConsole(wiringGpioLogEvent log);

void Log(wiringGpioLogLevel level, const char* sender, const char* function, const char* data);

void LogFormatted(wiringGpioLogLevel level, const char* sender, const char* function, const char* format, ...);