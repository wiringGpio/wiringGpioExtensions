#include <iostream>
#include <algorithm>
#include <sstream>
#include <wiringGpioExtensions.h>


#include "PinOnOffSample.h"
#include "../Logging.h"

using namespace std;


int main(int argc, char *argv[])
{
	WiringGpioSetLoggingCallback(WriteLogToConsole);
	
	int retVal = WiringGpioSetupPhys();
	if (retVal < 0)
		return retVal;
	
	return PinOnOffSampleMain(argc, argv);
}