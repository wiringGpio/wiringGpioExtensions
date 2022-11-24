//  Rotary Encoder Sample Project
//
#include <wiringGpioExtensions.h>


#include "EncoderSamples.h"
#include "RotaryEncoderSample.h"
#include "../Logging.h"



//  main
//
int main(int argc, char *argv[])
{
	WiringGpioSetLoggingCallback(WriteLogToConsole);
	
	int retVal = WiringGpioSetupPhys();
	
	//EncoderSampleMain(argc, argv);        
	
	RotaryEncoderSampleMain(argc, argv);
	
	WiringGpioTerminate();

}