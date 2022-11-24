#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "main.h"
#include "WiringPiExtensionApi.h"
#include "PCA9685Sample.h"
#include "PCA9685ServoSample.h"

using namespace std;


// Map of Pca chips, pin base to file descriptor
map<int, int> PcaMap;





void PcaAllOff()
{
	//  turn everything off
	map<int, int>::iterator it;
	for (it = PcaMap.begin(); it != PcaMap.end(); it++)
	{
		//  write the all off command (pin base + 16)
		DigitalWrite(it->first + 16, 0);
	}
}




int calcTicks(float impulseMs, float hertz) 
{ 
	float cycleMs = 1000.0f / hertz; 
	return (int)(MAX_PWM * impulseMs / cycleMs + 0.5f); 
} 



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
	
	
	//  Adafruit DC Motor Hat default address
	int fd = Pca9685Setup(600, 0x60, 50.0);
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
	
	
	
	ShutDownWiringPiExtension();
	
	return 0;
}