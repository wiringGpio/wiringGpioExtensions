#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <map>

#include <wiringGpioExtensions.h>

#include "MCP23000Sample.h"
#include "../Logging.h"

using namespace std;

//  Map of MCP chips, pin base to number of pins
map<int, int> McpMap;


#ifdef JETSON
int i2cBus = 0;
#else
int i2cBus = 1;
#endif


int main(int argc, char *argv[])
{
	WiringGpioSetLoggingCallback(WriteLogToConsole);
	
	int retVal = WiringGpioSetupPhys();
	if (retVal != 0)
		return retVal;
	
	//  Setup MCP chip
	Mcp23017Setup(i2cBus, 200, 0x23);
	McpMap[200] = 16;
	
	Mcp23008Setup(i2cBus, 300, 0x24);
	McpMap[300] =  8;
	
	Mcp23008Setup(i2cBus, 400, 0x27);
	McpMap[400] =  8;
	
	Mcp23017Setup(i2cBus, 700, 0x22);
	McpMap[700] = 16;
	
	//  Initialize all MCP pins for output in this sample
	map<int, int>::iterator it;
	for (it = McpMap.begin(); it != McpMap.end(); it++)
	{
		for (int i = 0; i < it->second; i++)
			PinMode(i + it->first, OUTPUT);
	}
	
	return Mcp23000SampleMain(argc, argv);
}
