#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "main.h"
#include "WiringPiExtensionApi.h"
#include "SoftwarePwmSample.h"
#include "HardwarePwmSample.h"

using namespace std;

//  Map of MCP chips, pin base to number of pins
map<int, int> McpMap;

// Map of Pca chips, pin base to file descriptor
map<int, int> PcaMap;




//  Prompt user with message and accept integer input from keyboard
int GetIntegerFromKeyboard(string message)
{
	string input;
	int number;
	
	while (true)
	{
		cout << message << " ";
		getline(cin, input);

		stringstream convertor;
		
		convertor << input;
		convertor >> number;

		if (convertor.fail())
		{
			// numberString is not a number!
			cout << "Please enter an integer ! " << endl;
		}
		else
			break;
	}
	
	return number;
}	







int main(int argc, char *argv[])
{
	//  setup wiring pi extension (which will init wiringPi library using physical pin numbers)
	int retVal = SetupWiringPiExtension();
	if (retVal != 0)
		return retVal;
	
	//  Initialize MCP chip
	Mcp23008Setup(400, 0x27);
	McpMap[400] =  8;
	
	//  Adafruit PWM Hat default address
	int fd = Pca9685Setup(500, 0x40, 50.0);
	if (fd != 0)
	{
		// Reset all output 
		Pca9685PWMReset(fd); 
		PcaMap[500] = fd;
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