#include <wiringGpioExtensions.h>
#include "MotorWithRotaryEncoderSample.h"



int main(int argc, char *argv[])
{
	//  setup wiring pi extension (which will init wiringPi library using physical pin numbers)
	int retVal = WiringGpioSetupPhys();
	
	if (retVal != 0)
		return retVal;
	
		
	return MotorWithRotaryEncoderSampleMain(argc, argv);
	
}
