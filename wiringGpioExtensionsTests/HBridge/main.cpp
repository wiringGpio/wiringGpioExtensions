#include <iostream>
#include <wiringGpioExtensions.h>

#include "HBridgeSample.h"


using namespace std;



int main(int argc, char *argv[])
{
	int retVal = WiringGpioSetupPhys();
	if (retVal != 0)
		return retVal;
	
		
	return HBridgeSampleMain(argc, argv);
	
}
