#include <map>
#include "wiringGpioExtensions.h"

#include <mcp23008.h>
#include <mcp23017.h>
#include <mcp3004.h>


using namespace std;


//  mcp23***.h
//
#pragma region MCP23xxx

//  MCP23008 port expansion chip
//
int Mcp23008Setup(int bus, int pinBase, int address)
{
	return mcp23008Setup(bus, pinBase, address);
}


//  MCP23017 port expansion chip
//
int Mcp23017Setup(int bus, int pinBase, int address)
{	
	return mcp23017Setup(bus, pinBase, address);
}

#pragma endregion


//  mcp3004.h
//
#pragma region MCP300x

//  MCP3004 Analog to Digital Converter
//  same code as 3008 but use specific function so we can keep track of pin base and pin number
int Mcp3004Setup(int pinBase, int spiChannel)
{
	return mcp3004Setup(pinBase, spiChannel);
}


//  MCP3008 Analog to Digital Converter
//   same code as 3004 but use specific function so we can keep track of pin base and pin number
int Mcp3008Setup(int pinBase, int spiChannel)
{	
	return mcp3004Setup(pinBase, spiChannel);
}

#pragma endregion

