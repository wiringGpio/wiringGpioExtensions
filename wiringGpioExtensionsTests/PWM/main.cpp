#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>

#include <wiringGpioExtensions.h>

#include "SoftwarePwmSample.h"
#include "HardwarePwmSample.h"
#include "../Logging.h"

using namespace std;


int main(int argc, char *argv[])
{
	WiringGpioSetLoggingCallback(WriteLogToConsole);
	
	int retVal = WiringGpioSetupPhys();
	if (retVal != 0)
		return retVal;
	
	//  Initialize MCP chip
	Mcp23008Setup(0, 400, 0x27);
	
	//  Adafruit PWM Hat default address
	int fd = Pca9685Setup(0, 500, 0x40, 50.0);
	if (fd != 0)
	{
		// Reset all output 
		Pca9685PWMReset(fd); 
	}
	else
	{
		return 1;
	}
	
	string input = " ";
	//  Main program loop
	while (input.compare("Q") != 0)
	{
		cout << "Enter Program Choice \n1) Software PWM\n2) Hardware PWM\nQ) Quit" << endl;
		getline(cin, input);
		std::transform(input.begin(), input.end(), input.begin(),::toupper); 
		
		if (input.compare("1") == 0)
		{
			SoftwarePwmSampleMain(argc, argv);
		}
		else if (input.compare("2") == 0)
		{
			HardwarePwmSampleMain(argc, argv);
		}
		else if (input.compare("Q") != 0)
		{
			cout << "Invalid input" << endl;
		}
	}
	
	return 0;
}