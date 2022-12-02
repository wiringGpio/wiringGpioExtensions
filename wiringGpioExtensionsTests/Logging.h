#pragma once
#include <wiringGpioExtensions.h>


//  Log level
extern wiringGpioLogLevel LogLevel;

void WriteLogToConsole(wiringGpioLogEvent log);
