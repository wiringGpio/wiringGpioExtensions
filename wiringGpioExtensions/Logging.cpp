

#define __STDC_WANT_LIB_EXT2__ 1  //Define you want TR 24731-2:2010 extensions

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdarg.h>

#ifdef JETSON
#include <wiringJet.h>
#else#include <wiringPi.h>
# endif

wiringGpioLoggingCallback LoggingFunction = 0x00;
wiringGpioLogLevel LogLevel = LogLevelAll;


//  Get the unix time in long long milliseconds
//
unsigned long long GetUnixTimeMilliseconds()
{
	struct timeval tv;
	gettimeofday(&tv, 0x00);
			
	return ((unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000);
}


//  Log Function
//
void AddLog(wiringGpioLogLevel level, const char* sender, const char* function, const char* data)
{
	if (LoggingFunction && level >= LogLevel)
	{
		wiringGpioLogEvent logItem;
		logItem.LogUnixTimeMilliseconds = GetUnixTimeMilliseconds();
		logItem.Level = level;
		logItem.Thread = 0;
		logItem.Sender = sender;
		logItem.Function = function;
		logItem.Data = data;
		
		LoggingFunction(logItem);
	}
}


//  Log function with string format
//
void AddLogFormatted(wiringGpioLogLevel level, const char* sender, const char* function, const char* format, ...)
{
	if (LoggingFunction && level >= LogLevel)
	{
		char* data;
		va_list args;

		va_start(args, format);
		if (0 > vasprintf(&data, format, args)) 
			data = 0x00;       //this is for logging, so failed allocation is not fatal
		va_end(args);

		if (data) 
		{
			AddLog(level, sender, function, data);
			free(data);
		}
		else 
		{
			AddLog(LogLevelWarn, sender, function, "Error while logging a message: Memory allocation failed.");
		}
	}
}