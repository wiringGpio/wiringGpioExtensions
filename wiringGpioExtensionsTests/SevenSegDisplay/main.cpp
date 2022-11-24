#include <wiringGpioExtensions.h>

#include "SevenSegDisplaySample.h"
#include "../Logging.h"



int main(int argc, char *argv[])
{
	WiringGpioSetLoggingCallback(WriteLogToConsole);
	
	//  setup wiring pi extension (which will init wiringPi library using physical pin numbers)
	int retVal = WiringGpioSetupPhys();

	// mcp chip
	Mcp23017Setup(0,700, 0x22);

	return SevenSegDisplaySampleMain(argc, argv);
	
	WiringGpioTerminate();
}