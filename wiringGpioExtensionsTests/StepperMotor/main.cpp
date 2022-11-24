#include <wiringGpioExtensions.h>

#include "StepperMotorSample.h"
#include "../Logging.h"

int main(int argc, char *argv[])
{
	WiringGpioSetLoggingCallback(WriteLogToConsole);
	
	//  setup wiring pi extension (which will init wiringPi library using physical pin numbers)
	int retVal = WiringGpioSetupPhys();
	
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