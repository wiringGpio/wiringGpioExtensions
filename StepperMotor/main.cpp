#include <iostream>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <string>
#include <sys/time.h>
#include <stdarg.h>
#include <iomanip>
#include <math.h>


#include "main.h"
#include "wiringGpioExtensions.h"
#include "StepperMotorSample.h"


using namespace std;


int GetIntegerFromKeyboard(string message)
{
	string input;
	int number;
	
	while (true)
	{
		cout << message << " ";
		getline(cin, input);

		stringstream convertor;
		
		convertor << input;
		convertor >> number;

		if (convertor.fail())
		{
			// numberString is not a number!
			cout << "Please enter an integer ! " << endl;
		}
		else
			break;
	}
	
	return number;
}	


float GetFloatFromKeyboard(string message)
{
	string input;
	float number;
	
	while (true)
	{
		cout << message << " ";
		getline(cin, input);

		stringstream convertor;

		convertor << input;
		convertor >> number;

		if (convertor.fail())
		{
			// numberString is not a number!
			std::cout << "Please enter a number !" << endl;
		}
		else
			break;
	}
	
	return number;
}	


#pragma region Logging

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


//  Write the log to the console
//
void LoggingFunction(wiringGpioLogEvent log)
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


//  Get the unix time in long long milliseconds
//
unsigned long long GetUnixTimeMilliseconds()
{
	struct timeval tv;
	gettimeofday(&tv, 0x00);
			
	return ((unsigned long long)(tv.tv_sec) * 1000 + (unsigned long long)(tv.tv_usec) / 1000);
}


//  Log function for the test app
//
void Log(wiringGpioLogLevel level, const char* sender, const char* function, const char* data)
{
	if (LogFunction && level >= LoggingLevel)
	{
		wiringGpioLogEvent logItem;
		logItem.LogUnixTimeMilliseconds = GetUnixTimeMilliseconds();
		logItem.Level = level;
		logItem.Thread = 0;
		logItem.Sender = sender;
		logItem.Function = function;
		logItem.Data = data;
		
		LogFunction(logItem);
	}
}


//  Log function for the test app with string format
//
void LogFormatted(wiringGpioLogLevel level, const char* sender, const char* function, const char* format, ...)
{
	if (LogFunction && level >= LoggingLevel)
	{
		char* data;
		va_list args;

		va_start(args, format);
		if (0 > vasprintf(&data, format, args)) data = 0x00;           //this is for logging, so failed allocation is not fatal
		va_end(args);

		if (data) {
			Log(level, sender, function, data);
			free(data);
		}
		else {
			Log(LogLevelWarn, sender, function, "Error while logging a message: Memory allocation failed.");
		}
	}
}


//  Logging callback function
wiringGpioLoggingCallback LogFunction = 0x00;
//  Log level
wiringGpioLogLevel LoggingLevel = LogLevelAll;

#pragma endregion	//  Logging





int main(int argc, char *argv[])
{
	wiringGpioSetLoggingCallback(LoggingFunction);
	LogFunction = LoggingFunction;
	
	//  setup wiring pi extension (which will init wiringPi library using physical pin numbers)
	int retVal = WiringGpioSetupPhys();
	
	//  setup MCP expander chips
	//	Mcp23017Setup(100, 0x20);
	//  McpMap[100] = 16
	Mcp23017Setup(0, 200, 0x23);
	
	Mcp23008Setup(0, 300, 0x24);
	
	Mcp23008Setup(0, 400, 0x27);
	
	Mcp23017Setup(0, 700, 0x22);
	
	//  Adafruit DC Motor Hat at default address
	int fd = Pca9685Setup(0, 600, 0x60, 50.0);
	if (fd != 0) 
	{
		// Reset all output 
		Pca9685PWMReset(fd); 
	}
	
	return StepperMotorSampleMain(argc, argv);
}