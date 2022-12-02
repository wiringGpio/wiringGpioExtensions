#include <iostream>
#include <sstream>
#include <string>
#include <sys/time.h>
#include <stdarg.h>
#include <iomanip>
#include <math.h>

#include "Logging.h"

using namespace std;

//  Get string for log level
//
string LogLevelString(wiringGpioLogLevel level)
{
	switch (level)
	{
	case 	LogLevelTrace:
		return "TRACE";
	case LogLevelDebug:
		return "DEBUG";
	case 	LogLevelInfo:
		return "INFO";
	case 	LogLevelWarn: 
		return "WARN";
	case 	LogLevelError:
		return "ERROR";
	case 	LogLevelFatal:
		return "FATAL";
	default:
		return "Unknown";
	}
}

//  Get the unix time in long long milliseconds
//
unsigned long long GetUnixTimeMilliseconds()
{
	struct timeval tv;
	gettimeofday(&tv, 0x00);
			
	return ((unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000);
}


//  Write the log to the console
//
void WriteLogToConsole(wiringGpioLogEvent log)
{
	timeval logTime;
	double seconds;
	double milliseconds = modf((double)log.LogUnixTimeMilliseconds / 1000, &seconds) * 1000;
	logTime.tv_sec = (int)seconds;
	logTime.tv_usec = (int)(milliseconds * 1000);
	tm* logLocalTime = localtime(&(logTime.tv_sec));

	ostringstream os;		
	os <<  setfill('0') << setw(2) << logLocalTime->tm_hour << ":" << setw(2) << logLocalTime->tm_min << ":" << setw(2) << logLocalTime->tm_sec <<  "." << std::setw(3) << logTime.tv_usec / 1000;
	os << setfill(' ') << "   "  << left << setw(7) << LogLevelString(log.Level) << " " << left << setw(25) << log.Sender << "  " << setw(35) << log.Function << "  " <<  log.Data;			

	
	cout << os.str() << endl;
	
}




