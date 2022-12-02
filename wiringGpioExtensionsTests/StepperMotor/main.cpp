#include <wiringGpioExtensions.h>

#include "StepperMotorSample.h"
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
	
	Mcp23017Setup(i2cBus, 200, 0x23);
	
	Mcp23008Setup(i2cBus, 300, 0x24);
	
	Mcp23008Setup(i2cBus, 400, 0x27);
	
	Mcp23017Setup(i2cBus, 700, 0x22);
	
	//  Adafruit DC Motor Hat at default address
	int fd = Pca9685Setup(i2cBus, 600, 0x60, 50.0);
	if (fd >= 0) 
	{
		// Reset all output 
		Pca9685PWMReset(fd); 
	}
	
	return StepperMotorSampleMain(argc, argv);
}