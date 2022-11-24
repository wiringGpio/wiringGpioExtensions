#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>

#include <wiringGpioExtensions.h>

#include "PCA9685Sample.h"
#include "PCA9685ServoSample.h"
#include "../Logging.h"
#include "../PwmHelpers.h"


using namespace std;

map<int, int> PcaMap;

int main(int argc, char *argv[])
{
	WiringGpioSetLoggingCallback(WriteLogToConsole);
	int retVal = WiringGpioSetupPhys();
	if (retVal != 0)
		return retVal;
	
	
	//  Adafruit DC Motor Hat default address
	int fd = Pca9685Setup(0, 600, 0x60, 50.0);
	if (fd != 0) 
	{
		// Reset all output 
		Pca9685PWMReset(fd); 
		PcaMap[600] = fd;
	}
	
	
//	//  Adafruit PWM Hat default address
//	fd = Pca9685Setup(500, 0x40, 50.0);
//	if (fd != 0)
//	{
//	// Reset all output 
//		Pca9685PWMReset(fd); 
//		PcaMap[500] = fd;
//	}
	
	string input = " ";
	//  Main program loop
	while (input.compare("Q") != 0)
	{
		cout << "Enter Program Choice \n1) PCA Pins On/Off\n2) PCA Servo Control\nQ) Quit" << endl;
		getline(cin, input);
		std::transform(input.begin(), input.end(), input.begin(), ::toupper); 
		
		if (input.compare("1") == 0)
		{
			PCA9685SampleMain(argc, argv);
		}
		else if (input.compare("2") == 0)
		{
			PCA9685ServoSampleMain(argc, argv);
		}
		else if (input.compare("Q") != 0)
		{
			cout << "Invalid input" << endl;
		}
	}
	
	
	
	WiringGpioTerminate();
	
	return 0;
}