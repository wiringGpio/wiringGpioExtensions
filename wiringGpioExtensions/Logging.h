#pragma once
#include <string>

#include <wiringGpioLogging.h>

// Logging Callback
extern wiringGpioLoggingCallback LogFunction;

//  Log Level
extern wiringGpioLogLevel LoggingLevel;

//  Add a log to the program logging
void Log(std::string sender, std::string function, std::string data, wiringGpioLogLevel level);