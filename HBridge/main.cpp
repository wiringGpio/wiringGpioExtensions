#include <iostream>
#include "main.h"
#include "WiringPiExtensionApi.h"
#include "HBridgeSample.h"

using namespace std;

//  Map of MCP chips, pin base to number of pins
map<int, int> McpMap;

int main(int argc, char *argv[])
{
	//  setup wiring pi extension (which will init wiringPi library using physical pin numbers)
	int retVal = SetupWiringPiExtension();
	if (retVal != 0)
		return retVal;
	
		
	return HBridgeSampleMain(argc, argv);
	
}
