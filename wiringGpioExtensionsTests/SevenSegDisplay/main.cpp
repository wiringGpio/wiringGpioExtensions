#include <wiringGpioExtensions.h>

#include "SevenSegDisplaySample.h"
#include "../Logging.h"

#ifdef JETSON
int i2cBus = 0;
#else
int i2cBus = 1;
#endif

int main(int argc, char *argv[])
{
	WiringGpioSetLoggingCallback(WriteLogToConsole);
	
	//  setup wiring pi extension (which will init wiringPi library using physical pin numbers)
	int retVal = WiringGpioSetupPhys();

	// mcp chip
	Mcp23017Setup(i2cBus,700, 0x22);

	return SevenSegDisplaySampleMain(argc, argv);
	
	WiringGpioTerminate();
}