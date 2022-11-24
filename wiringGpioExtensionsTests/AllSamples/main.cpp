#include <string>
#include <iostream>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <map>

#include <wiringGpioExtensions.h>


#include "../SevenSegDisplay/SevenSegDisplaySample.h"
#include "../StepperMotor/StepperMotorSample.h"
#include "../MCP23000/MCP23000Sample.h"
#include "../PWM/SoftwarePwmSample.h"
#include "../PWM/HardwarePwmSample.h"
#include "../HBridge/HBridgeSample.h"
#include "../PinOnOff/PinOnOffSample.h"
#include "../PCA9685/PCA9685Sample.h"
#include "../PCA9685/PCA9685ServoSample.h"
#include "../Logging.h"
#include "../Keyboard.h"
#include "../PwmHelpers.h"

using namespace std;

//  Map of MCP chips, pin base to number of pins
map<int, int> McpMap;
map<int, int> PcaMap;


int main(int argc, char *argv[])
{
	string input = " ";
	
	WiringGpioSetLoggingCallback(WriteLogToConsole);
	
	int retVal = WiringGpioSetupPhys();

	//  MCP chips on the GPIOJoy test board
	Mcp23017Setup(0,200, 0x23);
	McpMap[200] = 16;
	
	Mcp23008Setup(0,300, 0x24);
	McpMap[300] =  8;
	
	Mcp23008Setup(0,400, 0x27);
	McpMap[400] =  8;
	
	Mcp23017Setup(0,700, 0x22);
	McpMap[700] = 16;
	
	//  Extra chips
	Mcp23017Setup(0,800, 0x25);
	McpMap[800] = 16;
	
	//  Initialize MCP as all output for this sample
	map<int, int>::iterator it;
	for (it = McpMap.begin(); it != McpMap.end(); it++)
	{
		int pinBase = it->first;
		int numPins = it->second;
		for ( int i = 0; i < numPins; i++)
			PinMode(i+pinBase, PINMODE_OUTPUT);
	}
	
	
	//  Adafruit PWM Hat default address
	int fd = Pca9685Setup(0,500, 0x40, 50.0);
	if (fd != 0)
	{
	// Reset all output 
		Pca9685PWMReset(fd); 
		PcaMap[500] = fd;
	}
	
	//  Adafruit DC Motor Hat default address
	fd = Pca9685Setup(0,600, 0x60, 50.0);
	if (fd != 0) 
	{
		// Reset all output 
		Pca9685PWMReset(fd); 
		PcaMap[600] = fd;
	}
	
	
	//  Main program loop
	while (input.compare("Q") != 0)
	{
		cout << "Enter Program Choice \n1) Pin on/off\n2) Software PWM\n3) Hardware PWM\n4) PWM HBridge\n5) Stepper Motor\n6) MCP On/Off\n7) PCA9685 On/Off\n8) PCA9685 Servo\n9) Seven Segment Display\nq) Quit" << endl;
		getline(cin, input);
		std::transform(input.begin(), input.end(), input.begin(), ::toupper); 
		
		
		if (input.compare("1") == 0)
		{
			PinOnOffSampleMain(argc, argv);
		}
		if (input.compare("2") == 0)
		{
			SoftwarePwmSampleMain(argc, argv);
		}
		else if (input.compare("3") == 0)
		{
			HardwarePwmSampleMain(argc, argv);
		}
		else if (input.compare("4") == 0)
		{
			HBridgeSampleMain(argc, argv);
		}
		else if (input.compare("5") == 0)
		{
			StepperMotorSampleMain(argc, argv);
		}
		else if (input.compare("6") == 0)
		{
			Mcp23000SampleMain(argc, argv);
		}
		else if (input.compare("7") == 0)
		{
			PCA9685SampleMain(argc, argv);
		}
		else if (input.compare("8") == 0)
		{
			PCA9685ServoSampleMain(argc, argv);
		}
		else if (input.compare("9") == 0)
		{
			SevenSegDisplaySampleMain(argc, argv);
		}
		else if (input.compare("Q") != 0 )
		{
			cout << "Invalid input";
		}
	}	
	
	//  TODO ?McpAllOff();
	
	//  TODO ?PcaAllOff();
	
	WiringGpioTerminate();
}