#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include "main.h"
#include "WiringPiExtensionApi.h"
#include "HardwarePwmSample.h"


using namespace std;

#define PI_HARDWAREPWMPIN 12

int PromptHardwareSetupPwm(int& pinNumber, int& range)
{
	bool valid = false;
	string input;

	while (!valid)
	{
		cout << "Enter Pin Number, q to quit" << endl;
	
		getline(cin, input);

		if (input.compare("q") == 0)
			return false;

		pinNumber = strtol(input.c_str(), NULL, 10);

		//  this function recognizes the one hardware pin on the PI (GPIO18) and any PCA chip pins
		if (pinNumber == PI_HARDWAREPWMPIN)
			valid = true;
		else if (Pca9685GetFileDescriptorForPcaPin(pinNumber) != 0)
			valid = true;
		else
			cout << "Not a hardware PWM pin" << endl;
	}

	return true;
}



void PrintHardwarePwmInstructions()
{
	cout << endl;
	cout << "Pulse Width Modulation Control" << endl;
	cout << "W to increase PWM by 10%" << endl;
	cout << "S to decrease PWM by 10%" << endl;
	cout << "E to increase PWM by 1" << endl;
	cout << "D to decrease PWM by 1" << endl;
	cout << "R to change pin" << endl;
	cout << "X to input a value in range" << endl;
	cout << "Q to quit" << endl;
	cout << endl;
	
}


int HardwarePwmSampleMain(int argc, char *argv[])
{
	
	int range;
	int powerLevel = 0;
	int pinNumber = 0;  

	if (!PromptHardwareSetupPwm(pinNumber, range))
		return 0;
	
	if (pinNumber == PI_HARDWAREPWMPIN)
		range = 1024;
	else 
		range = 4096;
	
	PinMode(pinNumber, PINMODE_PWM_OUTPUT);
	
	cout << " PWM: pin " << pinNumber << " range " << range << " power level " << powerLevel << endl;

	bool repeat = true;
	while (repeat)
	{
		system("stty raw");  //  characters will be read as soon as they are input

		char key =  ' ';
		key = getchar();
		cout << '\b';		//  backspace the cursor
		
		key = toupper(key);
		
		//  handle key
		if (key == 'Q')
		{
			PwmWrite(pinNumber, 0);
			
			system("stty cooked");	// reset terminal to normal "cooked" mode 
			return 0;
		}
		else if (key == 'W')
		{
			powerLevel += (int)(range / 10);
		}
		else if (key == 'S')
		{
			powerLevel -= (int)(range / 10);
		}
		else if (key == 'E')
		{
			powerLevel += 1;
		}
		else if (key == 'D')
		{
			powerLevel -= 1;
		}
		else if (key == 'X')
		{	
			system("stty cooked");	
			cout << endl;
			powerLevel = GetIntegerFromKeyboard("Enter PWM value");
		}
		else if (key == 'R')
		{
			system("stty cooked");  
			cout << endl;

			int previousPin = pinNumber;
			if (!PromptHardwareSetupPwm(pinNumber, range))
			{
				return 0;
			}
			
			PinMode(pinNumber, PINMODE_PWM_OUTPUT);
		}
		else 
		{
			system("stty cooked");  
			PrintHardwarePwmInstructions();
			continue;
		}
		
		//  adjust for valid range
		if (powerLevel <= 0)
			powerLevel  = 0;
		else if (powerLevel > range)
			powerLevel = range;

		system("stty cooked");  

		cout << "Hardware PWM: pin " << pinNumber << " range " << range << " power level " << powerLevel << endl;
		
		PwmWrite(pinNumber, powerLevel);
	}


	return 0;
}